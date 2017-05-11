#ifndef POPULATION_H
#define POPULATION_H

#include "Neural_Network.h"

class Population
{
    int num_nets;
    vector< Neural_Network > nets;

public:

    Population( )
    {
    }

    Population( int in_num_nets )
    {
        num_nets = in_num_nets;
    }

    ~Population( )
    {
    }

    Population( const Population & p )
    {
        *this = p;
    }

    const Population & operator= ( const Population & p )
    {
        nets = p.nets;
        return *this;
    }

    void epoc( )
    {
        int death_count = num_nets * 0.2;
        for( int k = 0 ; k < death_count ; k = k + 1 )
        {
            nets[ ( num_nets - death_count ) + k ] = nets[ k ];
        }
    }
};


#endif
