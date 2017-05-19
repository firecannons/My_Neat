#ifndef XOR_GAME_H
#define XOR_GAME_H

#include "Neural_Network.h"
#include <cmath>

const double MAX_FITNESS = 100 ;

class Game
{
public:
    double run( Neural_Network & nn )
    {
        double fitness = 0;
        vector<double> input;
        vector<double> nn_output;
        vector<double> solution;
        input.resize( nn.get_num_input( ) );
        solution.resize( nn.get_num_output( ) );

        //printf( "weights: %f %f %f \n", nn.links[ 0 ].get_weight( ), nn.links[ 1 ].get_weight( ), nn.links[ 2 ].get_weight( ) ) ;
        //fflush( stdout ) ;

        //nn.links[ 2 ].set_weight( 0 );

        input[ 0 ] = 0;
        input[ 1 ] = 0;
        input[ 2 ] = 1;
        solution[ 0 ] = 0;
        nn_output = nn.run( input );
        fitness = fitness + ( MAX_FITNESS - fabs( solution[ 0 ] - nn_output[ 0 ] ) ) ;

        input[ 0 ] = 1;
        input[ 1 ] = 0;
        input[ 2 ] = 1;
        solution[ 0 ] = 1;
        nn_output = nn.run( input );
        fitness = fitness + ( MAX_FITNESS - fabs( solution[ 0 ] - nn_output[ 0 ] ) ) ;

        input[ 0 ] = 0;
        input[ 1 ] = 1;
        input[ 2 ] = 1;
        solution[ 0 ] = 1;
        nn_output = nn.run( input );
        fitness = fitness + ( MAX_FITNESS - fabs( solution[ 0 ] - nn_output[ 0 ] ) ) ;

        input[ 0 ] = 1;
        input[ 1 ] = 1;
        input[ 2 ] = 1;
        solution[ 0 ] = 2;
        nn_output = nn.run( input );
        fitness = fitness + ( MAX_FITNESS - fabs( solution[ 0 ] - nn_output[ 0 ] ) ) ;

        fitness = fitness / 4 ;

        return fitness;
    }
};

#endif
