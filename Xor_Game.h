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
        vector< unsigned int > runs ;
        for( unsigned int k = 0 ; k < 4 ; k = k + 1 )
            runs.push_back( k ) ;
        random_shuffle( runs.begin( ) , runs.end( ) ) ;

        for( unsigned int k = 0 ; k < 4 ; k = k + 1 )
        {
            if( runs[ k ] == 0 )
            {
                input[ 0 ] = 0;
                input[ 1 ] = 0;
                input[ 2 ] = 1;
                solution[ 0 ] = 0;
                nn_output = nn.run( input );
                fitness = fitness + ( MAX_FITNESS - fabs( solution[ 0 ] - nn_output[ 0 ] ) * MAX_FITNESS ) ;
            }

            if( runs[ k ] == 1 )
            {
                input[ 0 ] = 1;
                input[ 1 ] = 0;
                input[ 2 ] = 1;
                solution[ 0 ] = 1;
                nn_output = nn.run( input );
                fitness = fitness + ( MAX_FITNESS - fabs( solution[ 0 ] - nn_output[ 0 ] ) * MAX_FITNESS ) ;
            }

            if( runs[ k ] == 2 )
            {
                input[ 0 ] = 0;
                input[ 1 ] = 1;
                input[ 2 ] = 1;
                solution[ 0 ] = 1;
                nn_output = nn.run( input );
                fitness = fitness + ( MAX_FITNESS - fabs( solution[ 0 ] - nn_output[ 0 ] ) * MAX_FITNESS ) ;
            }

            if( runs[ k ] == 3 )
            {
                input[ 0 ] = 1;
                input[ 1 ] = 1;
                input[ 2 ] = 1;
                solution[ 0 ] = 0;
                nn_output = nn.run( input );
                fitness = fitness + ( MAX_FITNESS - fabs( solution[ 0 ] - nn_output[ 0 ] ) * MAX_FITNESS ) ;
            }
        }

        fitness = fitness / 4 ;

        fitness = fitness - ( nn.links.size( ) + nn.nodes.size( ) ) / 100 ;

        return fitness;
    }
};

#endif
