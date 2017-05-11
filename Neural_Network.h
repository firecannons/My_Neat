#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <vector>
#include "Node_Gene.h"
#include "Link_Gene.h"

using namespace std;

class Neural_Network
{
    vector< Node_Gene > nodes;
    vector< Link_Gene > links;

public:
    Neural_Network( )
    {
    }

    ~Neural_Network( )
    {
    }

    Neural_Network( int num_input, int num_hidden, int num_output )
    {
        nodes.resize( num_input + num_hidden + num_output ) ;

        int num_links = 0 ;
        num_links = num_input * num_output ;
        num_links = num_links + num_input * num_hidden ;
        num_links = num_links + num_hidden * num_output ;

        links.resize( num_links ) ;
    }

    Neural_Network( const Neural_Network & n )
    {
        *this = n;
    }

    const Neural_Network & operator= ( const Neural_Network & n )
    {
        nodes = n.nodes;
        links = n.links;
        return *this;
    }
};

#endif
