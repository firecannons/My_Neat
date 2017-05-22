#include "Population.h"
#include <iostream>
using namespace std;

const int NUM_NETS = 1000;
const int NUM_RUNS = 10;

int main()
{
    srand( time( NULL ) ) ;
    fflush( stdout ) ;
    Population p( NUM_NETS, 3, 2, 1 );

    printf( " after population constructor \n " ) ;
    fflush( stdout ) ;

    //p.nets[ 0 ]->load( "winner_net.nn" ) ;

    unsigned int generation = 0;
    while( p.get_best_fitness( ) < MAX_FITNESS - 0.5 )
    {
        p.epoc( NUM_RUNS ) ;
        printf( "Best fitness = %f of generation %u\n" , p.get_best_fitness(), generation );
        fflush( stdout ) ;
        generation = generation + 1;
    }

    //p.nets[ 0 ]->save( "winner_net.nn" ) ;

    while( true )
    {
        vector< double > input( 3 ) ;
        vector< double > output;
        cin >> input[ 0 ] ;
        cin >> input[ 1 ] ;
        cin >> input[ 2 ] ;
        output = p.nets[ 0 ]->run( input ) ;
        cout << output[ 0 ] << endl;
    }

    printf("Buffered, will be flushed");
    fflush(stdout);
    return 0;
}
