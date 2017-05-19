#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <vector>
#include <queue>
#include <set>
#include "Node_Gene.h"
#include "Link_Gene.h"
#include <cstdlib>

using namespace std;

const double LINK_SMALL_MUTATE_CHANCE = 0.15;
const double LINK_LARGE_MUTATE_CHANCE = 0.05;

const double NODE_FUNC_MUTATE_CHANCE = 0.05;

class Neural_Network
{
    vector < unsigned int > node_act_order;
    double fitness;
    int num_input;
    int num_hidden;
    int num_output;
    bool is_evaluated;

public:

    vector< Node_Gene > nodes;
    vector< Link_Gene > links;

    Neural_Network( )
    {
    }

    ~Neural_Network( )
    {
    }

    Neural_Network( unsigned int in_num_input, unsigned int in_num_hidden, unsigned int in_num_output )
    {
        num_input = in_num_input;
        num_hidden = in_num_hidden;
        num_output = in_num_output;
        fitness = 0;
        is_evaluated = false;

        nodes.resize( in_num_input + in_num_hidden + in_num_output ) ;

        unsigned int num_links = 0 ;
        num_links = num_links + in_num_input * in_num_hidden ;
        num_links = num_links + in_num_hidden * in_num_output ;
        num_links = in_num_input * in_num_output ;

        links.resize( num_links ) ;

        for( unsigned int k = 0 ; k < in_num_input ; k = k + 1 )
        {
            nodes[ k ].set_type( INPUT_TYPE );
        }
        for( unsigned int k = 0 ; k < in_num_hidden ; k = k + 1 )
        {
            nodes[ k + in_num_input ].set_type( HIDDEN_TYPE );
        }
        for( unsigned int k = 0 ; k < in_num_output ; k = k + 1 )
        {
            nodes[ k + in_num_input + in_num_hidden ].set_type( OUTPUT_TYPE );
        }
        for( unsigned int k = 0 ; k < in_num_input + in_num_hidden + in_num_output ; k = k + 1 )
        {
            nodes[ k ].set_act_func( FUNC_ZERO ) ;
        }

        unsigned int link_index = 0;
        for( unsigned int k = 0 ; k < nodes.size( ) ; k = k + 1 )
        {
            for( unsigned int k2 = k + 1 ; k2 < nodes.size( ) ; k2 = k2 + 1 )
            {
                if( nodes[ k ].get_type( ) != nodes[ k2 ].get_type( ) )
                {
                    links[ link_index ].set_in_neuron( k );
                    links[ link_index ].set_out_neuron( k2 );
                    nodes[ k ].add_out_link_index( link_index ) ;
                    nodes[ k2 ].add_in_link_index( link_index ) ;
                    link_index = link_index + 1;
                }
            }
        }


        for( unsigned int k = 0 ; k < num_links ; k = k + 1 )
        {
            links[ k ].set_current_value( 0 );
            links[ k ].set_weight( 1 );
        }

        topo_sort( ) ;
    }

    vector< double > run( vector<double> & input )
    {
        vector<double> output;
        output.resize( num_output ) ;

        unsigned int input_counter = 0;
        unsigned int output_counter = 0;
        for( unsigned int k = 0 ; k < node_act_order.size( ) ; k = k + 1 )
        {
            unsigned int cur_node_index = node_act_order[ k ] ;
            if( nodes[ cur_node_index ].get_type( ) == INPUT_TYPE )
            {
                double node_out_value = nodes[ cur_node_index ].run_act_func( input[ input_counter ] ) ;

                for( unsigned int k2 = 0 ; k2 < nodes[ cur_node_index ].out_link_indexes.size( ) ; k2 = k2 + 1 )
                {
                    //printf(" input sending to link weight: %f " , links[ nodes[ cur_node_index ].out_link_indexes[ k2 ] ].get_weight( ) );
                    //fflush( stdout );
                    unsigned int out_link_index = nodes[ cur_node_index ].out_link_indexes[ k2 ] ;

                    double out_link_value = node_out_value * links[ out_link_index ].get_weight( );

                    links[ out_link_index ].set_current_value( out_link_value ) ;
                }
                input_counter = input_counter + 1;
            }

            else if( nodes[ cur_node_index ].get_type( ) == HIDDEN_TYPE )
            {
                double sum = 0;

                sum = nodes[ cur_node_index ].sum_or_mult_up( links );

                double node_out_value = nodes[ cur_node_index ].run_act_func( sum ) ;
                for( unsigned int k2 = 0 ; k2 < nodes[ cur_node_index ].out_link_indexes.size( ) ; k2 = k2 + 1 )
                {
                    unsigned int out_link_index = nodes[ cur_node_index ].out_link_indexes[ k2 ] ;
                    links[ out_link_index ].set_current_value( node_out_value * links[ out_link_index ].get_weight( ) ) ;
                }
            }
            else if( nodes[ cur_node_index ].get_type( ) == OUTPUT_TYPE )
            {
                double sum = 0;

                sum = nodes[ cur_node_index ].sum_or_mult_up( links );

                output[ output_counter ] = sum ;

                output_counter = output_counter + 1;
            }
        }



        return output;
    }

    void topo_sort( )
    {
        set < unsigned int > done_nodes;
        queue < unsigned int > nodes_next;

        node_act_order.clear( ) ;
        for( unsigned int k = 0 ; k < nodes.size( ) ; k = k + 1 )
        {
            if( nodes[ k ].get_type( ) == INPUT_TYPE )
            {
                nodes_next.push( k ) ;
            }
        }


        while( ! nodes_next.empty( ) )
        {
            int current_node = nodes_next.front( );
            node_act_order.push_back( current_node ) ;
            for(unsigned int k = 0 ; k < nodes[ current_node ].out_link_indexes.size( ) ; k = k + 1 )
            {
                unsigned int out_link_index = nodes[ current_node ].out_link_indexes[ k ] ;
                unsigned int out_node = links[ out_link_index ].get_out_neuron( ) ;
                if( nodes[ out_node ].get_type() == HIDDEN_TYPE )
                {
                    if( done_nodes.count( out_node ) == 0 )
                    {
                        nodes_next.push( out_node ) ;
                        done_nodes.insert( out_node ) ;
                    }
                }
            }
            nodes_next.pop( ) ;
        }

        for( unsigned int k = 0 ; k < nodes.size( ) ; k = k + 1 )
        {
            if( nodes[ k ].get_type( ) == OUTPUT_TYPE )
            {
                node_act_order.push_back( k ) ;
            }
        }
    }

    void evolve_link_small_mutate( unsigned int k )
    {
        double rand_real = static_cast<double>( rand( ) ) / RAND_MAX ;
        if( rand_real < LINK_SMALL_MUTATE_CHANCE )
        {
            double rand_val = static_cast<double>( rand( ) ) ;
            links[ k ].set_weight( links[ k ].get_weight( ) - 0.1 + ( rand_val / RAND_MAX) * 0.2 ) ;
            //printf( "rand_val: %f   weight: %f  link: %u\n" , rand_val, links[ k ].get_weight( ), k ) ;
            //fflush( stdout );
        }
    }

    void evolve_link_large_mutate( unsigned int index )
    {
        double rand_real = 0;
        rand_real = static_cast<double>( rand( ) ) / RAND_MAX ;
        if( rand_real < LINK_LARGE_MUTATE_CHANCE )
        {
            double rand_val = static_cast<double>( rand( ) ) ;
            links[ index ].set_weight( links[ index ].get_weight( ) - 5 + ( rand_val / RAND_MAX) * 10 ) ;
            //printf( "rand_val: %f   weight: %f  link: %u\n" , rand_val, links[ k ].get_weight( ), k ) ;
            //fflush( stdout );
        }
    }

    void evolve_node_act_func( unsigned int k )
    {
        double rand_real = static_cast<double>( rand( ) ) / RAND_MAX ;
        if( rand_real < NODE_FUNC_MUTATE_CHANCE )
        {
            rand_real = static_cast<double>( rand( ) ) / RAND_MAX * ( FUNC_MAX - FUNC_MIN ) + FUNC_MIN ;
            nodes[ k ].set_act_func( round( rand_real ) ) ;
        }
    }

    void evolve_links( )
    {
        for( unsigned int k = 0 ; k < links.size( ) ; k = k + 1 )
        {
            evolve_link_small_mutate( k ) ;

            evolve_link_large_mutate( k ) ;
        }
    }

    void evolve_nodes( )
    {
        for( unsigned int k = 0 ; k < nodes.size( ) ; k = k + 1 )
        {
            evolve_node_act_func( k );
        }
    }

    void evolve( )
    {
        evolve_links( ) ;

        evolve_nodes( ) ;
    }

    Neural_Network( const Neural_Network & n )
    {
        *this = n;
    }

    const Neural_Network & operator= ( const Neural_Network & n )
    {
        nodes = n.nodes;
        links = n.links;
        fitness = n.fitness;
        num_input = n.num_input;
        num_hidden = n.num_hidden;
        num_output = n.num_output;
        is_evaluated = n.is_evaluated;
        node_act_order = n.node_act_order;
        return *this;
    }

    double get_fitness( )
    {
        return fitness ;
    }
    void set_fitness( double in_fitness )
    {
        fitness = in_fitness ;
    }
    int get_num_input( )
    {
        return num_input;
    }
    int get_num_output( )
    {
        return num_output;
    }
    void set_is_evaluated( bool in_is_evaluated ) { is_evaluated = in_is_evaluated ; }
    bool get_is_evaluated( ) { return is_evaluated ; }

    void reset( )
    {
        for( unsigned int k = 0 ; k < links.size( ) ; k = k + 1 )
        {
            links[ k ].set_current_value( 0 ) ;
        }
    }
};

#endif
