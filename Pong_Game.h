#ifndef PONG_GAME_H
#define PONG_GAME_H

#include "Neural_Network.h"

const double WINDOW_HEIGHT = 600;
const double WINDOW_WIDTH = 800;
const double BALL_RADIUS = 15;
const double PLAYER_WIDTH = 20;
const double PLAYER_DEFAULT_HEIGHT = 80;
const double BALL_X_START_SPEED = 10;
const double BALL_Y_START_SPEED = 10;
const double P1SPEED = 10;
// P2 is the Neural Network
// P1 is a shell AI that just moves with the ball
const double P2SPEED = 10;
const int LEFT_SIDE = 0;
const int RIGHT_SIDE = 1;

const unsigned int MAX_FITNESS = 10;


class Game;



vector< double > run_game_frame(Game & game, Neural_Network & net);
void game_draw(sf::RenderWindow & displayWindow, Game & game);

class Game
{
public:

    double p1y;
    double p2y;
    double p1h;
    double p2h;
    double ballx;
    double bally;
    double ballxspeed;
    double ballyspeed;
    int p1score;
    int p2score;
    double p1speed;
    double p2speed;
    int frame;

    Game()
    {
        init();
    }

    void init()
    {
        p1y = 0;
        p2y = 0;
        p1h = PLAYER_DEFAULT_HEIGHT;
        p2h = PLAYER_DEFAULT_HEIGHT;
        ballx = 0;
        bally = 0;
        ballxspeed = 0;
        ballyspeed = 0;
        p1score = 0;
        p2score = 0;
        p2speed = P2SPEED;
        p1speed = P1SPEED;
        frame = 0;
    }

    void start_game()
    {
        ballx = 0;
        bally = WINDOW_HEIGHT / 2;
        p1y = WINDOW_HEIGHT / 2;
        p2y = WINDOW_HEIGHT / 2;
        ballxspeed = BALL_X_START_SPEED;
        ballyspeed = BALL_Y_START_SPEED;
        p1score = 0;
        p2score = 0;
        p2speed = P2SPEED;
        p1speed = P1SPEED;
        frame = 0;
    }

    void start_after_goal(int side)
    {
        random_gen rng;
        if(side == LEFT_SIDE)
            ballxspeed = BALL_X_START_SPEED - rng.randreal(0, 0.2) * BALL_X_START_SPEED;
        else
            ballxspeed = -(BALL_X_START_SPEED - rng.randreal(0, 0.2) * BALL_X_START_SPEED);
        if( rng.randint(0, 1) )
            ballyspeed = BALL_Y_START_SPEED - rng.randreal(0, 0.2) * BALL_Y_START_SPEED;
        else
            ballyspeed = -(BALL_Y_START_SPEED - rng.randreal(0, 0.2) * BALL_Y_START_SPEED);

        bally = rng.randreal(0, 1) * (WINDOW_HEIGHT - BALL_RADIUS * 2);
    }

    double run( Neural_Network & nn )
    {
        double out = 0 ;
        init();
        start_game();
        vector< double > output ;
        while(p1score + p2score < MAX_FITNESS)
        {
            output = run_game_frame(*this, nn);
            if( std::isnan( output [ 0 ] ) )
                return 0;
        }
        out = p2score ;
        //out = out - static_cast<double>( ( nn.nodes.size( ) + nn.links.size( ) ) ) / 300 ;
        return out;
    }

    void show_off(Neural_Network & nn, sf::RenderWindow & MainWindow)
    {
        init();
        start_game();
        while(true)
        {
            run_game_frame(*this, nn);
            game_draw(MainWindow, *this);
        }
    }
};

/*
double abs(double x)
{
    if (x < 0) return -x;
    return x;
}*/

//std::vector<double>
vector< double > run_game_frame(Game & game, Neural_Network & net)
{
    vector< double > output ;
    std::vector<double> inputs;
    inputs.resize(9);

    inputs[0] = game.p1y;
    inputs[1] = game.p2y;
    inputs[2] = game.ballx;
    inputs[3] = game.bally;
    inputs[4] = game.ballxspeed;
    inputs[5] = game.ballyspeed;
    inputs[6] = WINDOW_WIDTH;
    inputs[7] = WINDOW_HEIGHT;
    inputs[8] = 1;
    output = net.run( inputs ) ;
    if( output[ 0 ] < -1 )
        output[ 0 ] = -1 ;
    if( output[ 0 ] > 1 )
        output[ 0 ] = 1 ;
    game.p2y = game.p2y + output[ 0 ] * P2SPEED;

    if(game.p1y + game.p1h / 2 < game.bally + BALL_RADIUS / 2)
        game.p1y = game.p1y + P1SPEED;
    else
        game.p1y = game.p1y - P1SPEED;

    game.ballx = game.ballx + game.ballxspeed;
    game.bally = game.bally + game.ballyspeed;

    game.ballxspeed = game.ballxspeed * 1.0005;
    game.ballyspeed = game.ballyspeed * 1.0005;

    if(game.bally < 0)
    {
        game.bally = 0;
        game.ballyspeed = -game.ballyspeed;
    }
    else if(game.bally + BALL_RADIUS > WINDOW_HEIGHT)
    {
        game.bally = WINDOW_HEIGHT - BALL_RADIUS;
        game.ballyspeed = -game.ballyspeed;
    }

    if(game.ballx < PLAYER_WIDTH)
    {
        game.ballx = PLAYER_WIDTH;
        game.ballxspeed = -game.ballxspeed;
        if(game.p1y > game.bally + BALL_RADIUS || game.p1y + game.p1h < game.bally)
        {
            game.p2score = game.p2score + 1;
            game.start_after_goal(0);
        }
    }
    else if(game.ballx + BALL_RADIUS > WINDOW_WIDTH - PLAYER_WIDTH)
    {
        game.ballx = WINDOW_WIDTH - PLAYER_WIDTH - BALL_RADIUS;
        game.ballxspeed = -game.ballxspeed;
        if(game.p2y > game.bally + BALL_RADIUS || game.p2y + game.p2h < game.bally)
        {
            game.p1score = game.p1score + 1;
            game.start_after_goal(1);
        }
    }
    return output ;
}



void game_draw(sf::RenderWindow & displayWindow, Game & game)
{
    printf( "game.p2y = %f\np2score = %u\np1score = %u" , game.p2y, game.p2score, game.p1score ) ;
    fflush( stdout ) ;
    sf::Font font;
    if (!font.loadFromFile("FreeSans.ttf"))
    {
        printf( "Error Loading Font!" ) ;
        fflush( stdout ) ;
    }

    sf::Text p1scoretext;
    p1scoretext.setFont(font); // font is a sf::Font
    char buf[ 1024 ] ;
    sprintf( buf , "%d", game.p1score ) ;
    p1scoretext.setString(buf);
    p1scoretext.setCharacterSize(24); // in pixels, not points!
    p1scoretext.setColor(sf::Color::Black);
    p1scoretext.setStyle(sf::Text::Bold | sf::Text::Underlined);
    displayWindow.draw(p1scoretext);

    sf::Text p2scoretext;
    p2scoretext.setFont(font); // font is a sf::Font
    char buf2[ 1024 ] ;
    sprintf( buf2 , "%d", game.p2score ) ;
    p2scoretext.setString(buf2);
    p2scoretext.setCharacterSize(24); // in pixels, not points!
    p2scoretext.setColor(sf::Color::Black);
    p2scoretext.setPosition(WINDOW_WIDTH - 150, 0);
    displayWindow.draw(p2scoretext);

    sf::RectangleShape ball(sf::Vector2f(BALL_RADIUS, BALL_RADIUS));
    sf::RectangleShape p1(sf::Vector2f(PLAYER_WIDTH, game.p1h));
    sf::RectangleShape p2(sf::Vector2f(PLAYER_WIDTH, game.p2h));

    ball.setFillColor(sf::Color(0, 0, 0));
    p1.setFillColor(sf::Color(0, 0, 0));
    p2.setFillColor(sf::Color(0, 0, 0));

    ball.setPosition(game.ballx, game.bally);
    p1.setPosition(0, game.p1y);
    p2.setPosition(displayWindow.getSize().x - PLAYER_WIDTH, game.p2y);

    displayWindow.draw(ball);
    displayWindow.draw(p1);
    displayWindow.draw(p2);

    displayWindow.display();

    displayWindow.clear(sf::Color(255, 255, 255));
}







#endif
