#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include "Node_Gene.h"
#include "Link_Gene.h"

using namespace std;

const double LINK_SMALL_MUTATE_CHANCE = 0.1;
const double LINK_LARGE_MUTATE_CHANCE = 0.02;

const double NODE_FUNC_MUTATE_CHANCE = 0.05;

const double ADD_NODE_CHANCE = 0.003; // Per link
const double ADD_LINK_CHANCE = 0.003; // Per pairs of nodes

const double MAX_WEIGHT = 5000 ;
const double MIN_WEIGHT = -5000 ;

const int NODE_MAX = 5000 ;
const int LINK_MAX = 25000 ;

class Neural_Network
{
    double fitness;
    int num_input;
    int num_hidden;
    int num_output;
    bool is_evaluated;

public:

    vector< Node_Gene > nodes;
    vector< Link_Gene > links;
    vector < unsigned int > node_act_order;

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
        num_links = num_links + in_num_input * in_num_output ;

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
            nodes[ k ].set_act_func( FUNC_LINEAR ) ;
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


        /*for( unsigned int k = 0 ; k < num_links ; k = k + 1 )
        {
            links[ k ].set_current_value( 0 );
            links[ k ].set_weight( 0 );
        }

        for( unsigned int k = 0 ; k < node_act_order.size( ) ; k = k + 1 )
        {
            printf( " %u %u , " , node_act_order[ k ], nodes[ node_act_order[ k ] ].get_act_func( ) ) ;
            fflush( stdout ) ;
        }
        printf( " = before\n" ) ;
        fflush( stdout ) ;*/

        topo_sort( ) ;

        /*for( unsigned int k = 0 ; k < node_act_order.size( ) ; k = k + 1 )
        {
            printf( " %u %u , " , node_act_order[ k ], nodes[ node_act_order[ k ] ].get_act_func( ) ) ;
            fflush( stdout ) ;
        }
        printf( " =after\n" ) ;
        fflush( stdout ) ;*/
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
        if( links[ k ].get_weight( ) > MAX_WEIGHT )
        {
            links[ k ].set_weight( MAX_WEIGHT ) ;
        }
        if( links[ k ].get_weight( ) < MIN_WEIGHT )
        {
            links[ k ].set_weight( MIN_WEIGHT ) ;
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
        if( links[ index ].get_weight( ) > MAX_WEIGHT )
        {
            links[ index ].set_weight( MAX_WEIGHT ) ;
        }
        if( links[ index ].get_weight( ) < MIN_WEIGHT )
        {
            links[ index ].set_weight( MIN_WEIGHT ) ;
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

    void evolve_add_nodes( unsigned int k )
    {
        if( links.size( ) < LINK_MAX )
        {
            if( nodes.size( ) < NODE_MAX )
            {
                double rand_real = static_cast<double>( rand( ) ) / RAND_MAX ;
                if( rand_real < ADD_NODE_CHANCE )
                {
                    links.push_back( Link_Gene( ) ) ;
                    links.push_back( Link_Gene( ) ) ;
                    nodes.push_back( Node_Gene( ) ) ;

                    links[ links.size( ) - 2 ].set_current_value( 0 ) ;
                    links[ links.size( ) - 2 ].set_weight( static_cast<double>( rand( ) ) / RAND_MAX * 5 - 10 ) ;

                    links[ links.size( ) - 1 ].set_current_value( 0 ) ;
                    links[ links.size( ) - 1 ].set_weight( static_cast<double>( rand( ) ) / RAND_MAX * 5 - 10 ) ;

                    links[ links.size( ) - 2 ].set_weight( 0 ) ;
                    links[ links.size( ) - 1 ].set_weight( 0 ) ;
                    links[ k ].set_weight( 0 ) ;

                    nodes[ nodes.size( ) - 1 ].set_act_func( FUNC_TANH ) ;
                    nodes[ nodes.size( ) - 1 ].set_type( HIDDEN_TYPE ) ;

                    nodes[ nodes.size( ) - 1 ].add_in_link_index( links.size( ) - 2 ) ;
                    nodes[ nodes.size( ) - 1 ].add_out_link_index( links.size( ) - 1 ) ;

                    nodes[ links[ k ].get_in_neuron( ) ].add_out_link_index( links.size( ) - 2 ) ;
                    nodes[ links[ k ].get_out_neuron( ) ].add_in_link_index( links.size( ) - 1 ) ;

                    links[ links.size( ) - 2 ].set_in_neuron( links[ k ].get_in_neuron( ) ) ;
                    links[ links.size( ) - 2 ].set_out_neuron( nodes.size( ) - 1 ) ;
                    links[ links.size( ) - 1 ].set_in_neuron( nodes.size( ) - 1 ) ;
                    links[ links.size( ) - 1 ].set_out_neuron( links[ k ].get_out_neuron( ) ) ;
                }
            }
        }
    }

    void evolve_links( )
    {
        for( unsigned int k = 0 ; k < links.size( ) ; k = k + 1 )
        {
            evolve_link_small_mutate( k ) ;

            evolve_link_large_mutate( k ) ;

            evolve_add_nodes( k ) ;
        }
    }

    void evolve_nodes( )
    {
        for( unsigned int k = 0 ; k < nodes.size( ) ; k = k + 1 )
        {
            evolve_node_act_func( k );

            evolve_add_link( k ) ;
        }
    }

    void evolve_add_link( unsigned int k )
    {
        if( links.size( ) < LINK_MAX )
        {
            for( unsigned int k2 = 0 ; k2 < nodes.size( ) ; k2 = k2 + 1 )
            {
                double rand_real = static_cast<double>( rand( ) ) / RAND_MAX ;
                if( rand_real < ADD_LINK_CHANCE )
                {
                    bool link_currently = false ;
                    for( unsigned int k3 = 0 ; k3 < nodes[ k ].out_link_indexes.size( ) ; k3 = k3 + 1 )
                    {
                        if( links[ nodes[ k ].out_link_indexes[ k3 ] ].get_out_neuron( ) == k2 )
                            link_currently = true;
                    }

                    //if( link_currently == false )
                    {
                        links.push_back( Link_Gene() );
                        links[ links.size( ) - 1 ].set_weight( 0 ) ;
                        links[ links.size( ) - 1 ].set_in_neuron( k ) ;
                        links[ links.size( ) - 1 ].set_out_neuron( k2 ) ;
                        nodes[ k ].out_link_indexes.push_back( links.size( ) - 1 ) ;
                        nodes[ k2 ].in_link_indexes.push_back( links.size( ) - 1 ) ;
                    }
                }
            }
        }
    }

    void evolve( )
    {
        evolve_links( ) ;

        evolve_nodes( ) ;

        topo_sort( ) ;
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

    void save( string file_name )
    {
        ofstream fout ;
        fout.open( file_name.c_str( ) ) ;
        fout << nodes.size( ) << endl ;
        fout << links.size( ) << endl ;
        for( unsigned int k = 0 ; k < nodes.size( ) ; k = k + 1 )
        {
            fout << "node " << nodes[ k ].get_act_func() << " " << nodes[ k ].get_type( ) << endl;
            fout << nodes[ k ].in_link_indexes.size( ) << " " ;
            for( unsigned int k2 = 0 ; k2 < nodes[ k ].in_link_indexes.size( ) ; k2 = k2 + 1 )
            {
                fout << nodes[ k ].in_link_indexes[ k2 ] << " " ;
            }
            fout << endl;
            fout << nodes[ k ].out_link_indexes.size( ) << " " ;
            for( unsigned int k2 = 0 ; k2 < nodes[ k ].out_link_indexes.size( ) ; k2 = k2 + 1 )
            {
                fout << nodes[ k ].out_link_indexes[ k2 ] << " " ;
            }
            fout << endl;
        }
        for( unsigned int k = 0 ; k < links.size( ) ; k = k + 1 )
        {
            fout << "link " << links[ k ].get_weight( ) << " " ;
            fout << links[ k ].get_in_neuron( ) << " " << links[ k ].get_out_neuron( ) << " " ;
            fout << links[ k ].get_enabled( ) << " " << links[ k ].get_innov_number( ) << endl ;
        }
        fout.close( ) ;
    }

    void load( string file_name )
    {
        string object_title;
        unsigned int num_nodes = 0;
        unsigned int num_links = 0;

        ifstream fin ;
        fin.open( file_name.c_str() ) ;

        fin >> num_nodes ;
        fin >> num_links ;

        nodes.clear( ) ;
        links.clear( ) ;
        node_act_order.clear( ) ;

        nodes.resize( num_nodes ) ;
        links.resize( num_links ) ;

        for( unsigned int k = 0 ; k < nodes.size( ) ; k = k + 1 )
        {
            unsigned int node_act_func = 0;
            unsigned int node_type = 0;
            unsigned int num_in_link_indexes = 0;
            unsigned int num_out_link_indexes = 0;

            fin >> object_title ;
            fin >> node_act_func ;
            fin >> node_type ;
            nodes[ k ].set_act_func( node_act_func ) ;
            nodes[ k ].set_type( node_type ) ;
            fin >> num_in_link_indexes ;
            for( unsigned int k2 = 0 ; k2 < num_in_link_indexes ; k2 = k2 + 1 )
            {
                unsigned int in_link_index ;
                fin >> in_link_index ;
                nodes[ k ].add_in_link_index( in_link_index ) ;
            }
            fin >> num_out_link_indexes ;
            for( unsigned int k2 = 0 ; k2 < num_out_link_indexes ; k2 = k2 + 1 )
            {
                unsigned int out_link_index ;
                fin >> out_link_index ;
                nodes[ k ].add_out_link_index( out_link_index ) ;
            }
        }
        for( unsigned int k = 0 ; k < links.size( ) ; k = k + 1 )
        {
            double fin_weight ;
            unsigned int fin_in_neuron ;
            unsigned int fin_out_neuron ;
            bool fin_enabled ;
            unsigned int fin_innov ;

            fin >> object_title ;
            fin >> fin_weight ;
            fin >> fin_in_neuron ;
            fin >> fin_out_neuron ;
            fin >> fin_enabled ;
            fin >> fin_innov ;

            links[ k ].set_weight( fin_weight ) ;
            links[ k ].set_in_neuron( fin_in_neuron ) ;
            links[ k ].set_out_neuron( fin_out_neuron ) ;
            links[ k ].set_enabled( fin_enabled ) ;
            links[ k ].set_fin_innov( fin_innov ) ;

        }

        fin.close( ) ;
        set_is_evaluated( false ) ;
        set_fitness( 0 ) ;
        topo_sort( ) ;
    }

    void reset( )
    {
        for( unsigned int k = 0 ; k < links.size( ) ; k = k + 1 )
        {
            links[ k ].set_current_value( 0 ) ;
        }
    }
};

#endif
