#ifndef POPULATION_H
#define POPULATION_H

const double KILL_AMOUNT = 0.3;
const double MUTATE_AMOUNT = 0.8;

const bool SKIP_NET_IF_EVALUATED = true;

#include "Neural_Network.h"
#include "Pong_Game.h"

class Population
{
public:

    vector< Neural_Network * > nets;
    double best_fitness ;

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
        nn.set_is_evaluated( true ) ;
        /*if( nn.nodes.size( ) > 4 )
        {
            printf( "ooh, %u nodes %u links %u node_act_order_size \n" , nn.nodes.size( ), nn.links.size( ) , nn.node_act_order.size( ) ) ;
            fflush( stdout ) ;
            printf( "ooh, %f average \n " , average ) ;
            fflush( stdout ) ;
            for( unsigned int k = 0 ; k < nn.node_act_order.size( ) ; k = k + 1 )
            {
                printf( " %u %u , " , nn.node_act_order[ k ], nn.nodes[ nn.node_act_order[ k ] ].get_act_func( ) ) ;
                fflush( stdout ) ;
            }
            printf( "\n" ) ;
            fflush( stdout ) ;
        }
        for( unsigned int k = 0 ; k < nn.nodes[ 3 ].in_link_indexes.size( ) ; k = k + 1 )
        {
            printf( " %u , " , nn.links[ nn.nodes[ 3 ].in_link_indexes[ k ] ].get_in_neuron( ) ) ;
            fflush( stdout ) ;
        }
        printf( "\n" ) ;
        fflush( stdout ) ;*/
        return average;
    }

    void epoc( int num_runs )
    {
        //#pragma omp parallel for
        for(unsigned int k = 0; k < nets.size( ); k = k + 1)
        {
            if( nets[ k ]->get_is_evaluated( ) == false || SKIP_NET_IF_EVALUATED == false )
            {
                double average = 0;

                average = average_over_runs( num_runs, *( nets[ k ] ) );

                nets[ k ]->set_fitness( average );
            }
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

        printf( " num of nodes in best: %u     num links in best: %u \n" , nets[ 0 ]->nodes.size( ), nets[ 0 ]->links.size( ) ) ;
        fflush( stdout ) ;

        unsigned int mutate_count = nets.size( ) * MUTATE_AMOUNT;
        unsigned int mutate_start_index = nets.size( ) - mutate_count ;
        evolve( mutate_start_index );
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
            nets[ k ]->set_is_evaluated( false ) ;
        }
    }

    double get_best_fitness( ) { return best_fitness ; }
};


#endif
