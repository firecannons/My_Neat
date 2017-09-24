#include "Population.h"

const int NUM_NETS = 100;
const int NUM_RUNS = 1;

int main()
{
    srand( time( NULL ) ) ;
    fflush( stdout ) ;
    Population p( NUM_NETS, 9, 0, 1 );

    printf( " after population constructor \n " ) ;
    fflush( stdout ) ;

    //p.nets[ 0 ]->load( "winner_net.nn" ) ;

    unsigned int generation = 0;
    while( p.get_best_fitness( ) < MAX_FITNESS )
    {
        p.epoc( NUM_RUNS ) ;
        printf( "Best fitness = %f <= ( MAX_FITNESS = %u) of generation %u\n" , p.get_best_fitness(), MAX_FITNESS, generation );
        fflush( stdout ) ;
        generation = generation + 1;
        p.nets[ 0 ]->save( "winner_net.nn" ) ;
    }

    sf::RenderWindow main_window( sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ) , "SFML window") ;
    main_window.setFramerateLimit(60) ;
    Game game ;
    game.show_off( * ( p.nets[ 0 ] ) , main_window ) ;

    printf("Buffered, will be flushed");
    fflush(stdout);
    return 0;
}
