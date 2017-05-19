#ifndef LINK_GENE_H
#define LINK_GENE_H

class Link_Gene
{
    double weight;
    double current_value;
    bool enabled;
    int innov;
    int in_neuron;
    int out_neuron;

public:

    Link_Gene( )
    {

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

    int get_in_neuron( )
    {
        return in_neuron;
    }

    void set_in_neuron( int in_in_neuron )
    {
        in_neuron = in_in_neuron;
    }

    int get_out_neuron( )
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

};

#endif
