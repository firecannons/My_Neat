#ifndef NODE_GENE_H
#define NODE_GENE_H

#include <vector>
#include <cmath>
#include "Link_Gene.h"
using namespace std;

const int INPUT_TYPE = 0;
const int HIDDEN_TYPE = 1;
const int OUTPUT_TYPE = 2;

const int FUNC_HEAVISIDE = 0;
const int FUNC_RELU = 1;
const int FUNC_SIGMOID = 2;
const int FUNC_TANH = 3;
const int FUNC_GAUSSIAN = 4;
const int FUNC_SINE = 5;
const int FUNC_ABS = 6;
const int FUNC_SQUARE = 7;
const int FUNC_MULT = 8;
const int FUNC_LINEAR = 9;
const int FUNC_ZERO = 10;
const int FUNC_MAX = 10;
const int FUNC_MIN = 0;

const double E = 2.71828;

double Heaviside( double input );
double ReLU( double input ) ;
double Sigmoid( double input ) ;
double TanH( double input ) ;
double gaussian( double input ) ;
double sine( double input ) ;
double square( double input ) ;
double zero( double input ) ;

class Node_Gene
{
    int type;
    int act_func;

public:
    vector< unsigned int > in_link_indexes ;
    vector< unsigned int > out_link_indexes ;

    Node_Gene( )
    {
    }

    ~Node_Gene( )
    {
    }

    Node_Gene( const Node_Gene & ng )
    {
        *this = ng;
    }

    const Node_Gene & operator= ( const Node_Gene & ng )
    {
        type = ng.type;
        act_func = ng.act_func;
        in_link_indexes = ng.in_link_indexes;
        out_link_indexes = ng.out_link_indexes;
        return *this;
    }

    void set_type( int in_type )
    {
        type = in_type;
    }

    int get_type( )
    {
        return type;
    }

    double run_act_func( double input_value )
    {
        double output_value = input_value;
        if( act_func == FUNC_HEAVISIDE )
        {
            output_value = Heaviside( input_value ) ;
        }
        if( act_func == FUNC_RELU )
        {
            output_value = ReLU( input_value ) ;
        }
        if( act_func == FUNC_SIGMOID )
        {
            output_value = Sigmoid( input_value ) ;
        }
        if( act_func == FUNC_TANH )
        {
            output_value = TanH( input_value ) ;
        }
        if( act_func == FUNC_GAUSSIAN )
        {
            output_value = gaussian( input_value ) ;
        }
        if( act_func == FUNC_SINE )
        {
            output_value = sine( input_value ) ;
        }
        if( act_func == FUNC_ABS )
        {
            output_value = fabs( input_value ) ;
        }
        if( act_func == FUNC_SQUARE )
        {
            output_value = square( input_value ) ;
        }
        if( act_func == FUNC_ZERO )
        {
            output_value = zero( input_value );
        }

        if( act_func == FUNC_MULT )
        {
            output_value = input_value;
        }
        return output_value;
    }

    double sum_or_mult_up( vector < Link_Gene > & links )
    {
        double output_value = 0;
        if( act_func != FUNC_MULT )
        {
            double sum = 0;
            for( unsigned int k2 = 0 ; k2 < in_link_indexes.size( ) ; k2 = k2 + 1 )
            {
                sum = sum + links[ in_link_indexes[ k2 ] ].get_current_value( ) ;
            }
            output_value = sum;
        }
        else
        {
            double product = 0;
            for( unsigned int k2 = 0 ; k2 < in_link_indexes.size( ) ; k2 = k2 + 1 )
            {
                product = product * links[ in_link_indexes[ k2 ] ].get_current_value( ) ;
            }
            output_value = product;
        }
        return output_value ;
    }

    double activate( vector < Link_Gene > & links)
    {
        double output_value = 0;
        double sum = 0;

        sum = sum_or_mult_up( links );

        output_value = run_act_func( sum );

        return output_value ;
    }

    void set_act_func( int in_act_func ) { act_func = in_act_func ; }
    int get_act_func( ) { return act_func ; }
    void add_in_link_index( unsigned int index ) { in_link_indexes.push_back( index ) ; }
    void add_out_link_index( unsigned int index ) { out_link_indexes.push_back( index ) ; }
};

double Heaviside( double input )
{
    return input > 0.5 ? 1 : 0 ;
}

double ReLU( double input )
{
    double output = 0;
    output = input;
    if( output < 0 )
        output = 0;
    return output;
}

double Sigmoid( double input )
{
    double output = 0;

    output = 1 / ( 1 + pow( E, - input ) ) ;

    return output;
}

double TanH( double input )
{
    double output = tanh( input ) ;

    return output ;
}

double gaussian( double input )
{
    double output = 0 ;

    output = pow( E, - ( input * input ) ) ;

    return output ;
}

double sine( double input )
{
    double output = 0;
    output = sin( input );
    return output;
}

double square( double input )
{
    double output = 0 ;
    output = input * input ;
    return output ;
}

double zero( double input )
{
    double output = 0 ;
    output = 0;
    return output;
}

#endif
