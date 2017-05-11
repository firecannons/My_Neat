#include <cstdio>
#include "Population.h"

const int NUM_NETS = 100;

int main()
{
    Population p(NUM_NETS);

    printf("Buffered, will be flushed");
    fflush(stdout);
    return 0;
}
