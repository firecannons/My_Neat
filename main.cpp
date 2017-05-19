#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "Population.h"

const int NUM_NETS = 100;
const int NUM_RUNS = 10;

int main()
{
    srand( time( NULL ) ) ;
    fflush( stdout ) ;
    Population p( NUM_NETS, 3, 0, 1 );

    printf( " after population constructor \n " ) ;
    fflush( stdout ) ;

    unsigned int generation = 0;
    while( p.get_best_fitness( ) < MAX_FITNESS - 0.01 )
    {
        p.epoc( NUM_RUNS ) ;
        printf( "Best fitness = %f of generation %u\n" , p.get_best_fitness(), generation );
        fflush( stdout ) ;
        generation = generation + 1;
    }

    printf("Buffered, will be flushed");
    fflush(stdout);
    return 0;
}
