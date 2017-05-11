#ifndef NODE_GENE_H
#define NODE_GENE_H

const int INPUT_TYPE = 0;
const int HIDDEN_TYPE = 1;
const int OUTPUT_TYPE = 2;

class Node_Gene
{
    int type;

public:

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
};

#endif
