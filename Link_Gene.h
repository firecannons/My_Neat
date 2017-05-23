#ifndef LINK_GENE_H
#define LINK_GENE_H

#include <vector>
#include <queue>
#include <set>
#include <cstdio>
//#include <cstdlib>
//#include <ctime>
#include <algorithm>
#include <fstream>
#include <cmath>
#include "SFML/Graphics.hpp"
#include "Random_gen.h"

class Link_Gene
{
    double weight;
    double current_value;
    bool enabled;
    unsigned innov;
    unsigned int in_neuron;
    unsigned int out_neuron;

public:

    Link_Gene( )
    {
        enabled = true;
        innov = 0;
    }

    ~Link_Gene( )
    {

    }

    Link_Gene( const Link_Gene & lg )
    {
        *this = lg;
    }

    const Link_Gene & operator= ( const Link_Gene & lg )
    {
        in_neuron = lg.in_neuron;
        out_neuron = lg.out_neuron;
        weight = lg.weight;
        enabled = lg.enabled;
        innov = lg.innov;
        current_value = lg.current_value;
        return *this;
    }

    unsigned int get_in_neuron( )
    {
        return in_neuron;
    }

    void set_in_neuron( int in_in_neuron )
    {
        in_neuron = in_in_neuron;
    }

    unsigned int get_out_neuron( )
    {
        return out_neuron;
    }

    void set_out_neuron( int in_out_neuron )
    {
        out_neuron = in_out_neuron;
    }

    double get_weight( )
    {
        return weight;
    }

    void set_weight( double in_weight )
    {
        weight = in_weight;
    }

    void set_current_value( double in_current_value ) { current_value = in_current_value ; }
    double get_current_value( ) { return current_value ; }
    unsigned int get_innov_number( ) { return innov ; }
    bool get_enabled( ) { return enabled ; }
    void set_enabled( bool in_enabled ) { enabled = in_enabled ; }
    void set_fin_innov( bool in_innov ) { innov = in_innov ; }
};

#endif
