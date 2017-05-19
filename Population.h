#ifndef POPULATION_H
#define POPULATION_H

const double KILL_AMOUNT = 0.2;

#include "Neural_Network.h"
#include "Xor_Game.h"

class Population
{
    vector< Neural_Network * > nets;
    double best_fitness ;

public:

    Population( )
    {
    }

    Population( unsigned int in_num_nets, unsigned int num_input, unsigned int num_hidden, unsigned int num_output )
    {
        nets.resize( in_num_nets ) ;
        best_fitness = 0;
        for( unsigned int k = 0 ; k < nets.size( ) ; k = k + 1 )
        {
            nets[ k ] = new Neural_Network( num_input , num_hidden , num_output ) ;
        }
    }

    ~Population( )
    {
        for( unsigned int k = 0 ; k < nets.size( ) ; k = k + 1 )
        {
            delete nets[ k ] ;
        }
    }

    Population( const Population & p )
    {
        *this = p;
    }

    const Population & operator= ( const Population & p )
    {
        for(unsigned int k = 0 ; k < nets.size( ) ; k = k + 1 )
        {
            delete nets[ k ];
        }
        nets.resize( p.nets.size( ) ) ;
        for(unsigned int k = 0 ; k < nets.size( ) ; k = k + 1 )
        {
            nets[ k ] = new Neural_Network( *( p.nets[ k ] ) ) ;
        }
        best_fitness = p.best_fitness ;
        return *this;
    }

    void my_sort( )
    {
        for( unsigned int k = 0 ; k < nets.size( ) ; k = k + 1 )
        {
            for( unsigned int k2 = nets.size( ) - 1 ; k2 > k ; k2 = k2 - 1 )
            {
                if( nets[ k2 - 1 ]->get_fitness( ) < nets[ k2 ]->get_fitness( ) )
                {
                    Neural_Network * tmp;
                    tmp = nets[ k2 - 1 ];
                    nets[ k2 - 1 ] = nets[ k2 ];
                    nets[ k2 ] = tmp;
                }
            }
        }
    }

    double average_over_runs( unsigned int num_runs, Neural_Network & nn )
    {
        Game game;
        double average = 0;
        for(unsigned int k = 0; k < num_runs; k = k + 1)
        {
            average = average + game.run( nn );
            nn.reset( );
        }
        average = average / num_runs;
        return average;
    }

    void epoc( int num_runs )
    {
        for(unsigned int k = 0; k < nets.size( ); k = k + 1)
        {
            double average = 0;

            average = average_over_runs( num_runs, *( nets[ k ] ) );

            nets[ k ]->set_fitness( average );
        }



        /*for( unsigned int k = 0 ; k < nets.size( ) ; k = k + 1 )
        {
            printf( "%f\n", nets[ k ]->get_fitness( ) ) ;
            fflush( stdout ) ;
        }*/

        my_sort( ) ;

        best_fitness = nets[ 0 ]->get_fitness( ) ;

        /*for( unsigned int k = 0 ; k < nets.size( ) ; k = k + 1 )
        {
            printf("%f\n", nets[ k ]->get_fitness( ) ) ;
            fflush( stdout ) ;
        }*/

        unsigned int death_count = nets.size( ) * KILL_AMOUNT;
        unsigned int start_index = nets.size( ) - death_count ;
        for( unsigned int k = 0 ; k < death_count ; k = k + 1 )
        {
            delete nets[ start_index + k ] ;
            nets[ start_index + k ] = new Neural_Network( *( nets[ k ] ) ) ;
        }

        evolve( start_index );
    }

    void reset_nets( )
    {
        for( unsigned int k = 0 ; k < nets.size( ) ; k = k + 1 )
        {
            nets[ k ]->reset( ) ;
        }
    }

    void evolve( unsigned int start_index )
    {
        for( unsigned int k = start_index ; k < nets.size( ) ; k = k + 1 )
        {
            nets[ k ]->evolve( );
        }
    }

    double get_best_fitness( ) { return best_fitness ; }
};


#endif
