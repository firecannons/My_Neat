#ifndef RANDOM_GEN_H
#define RANDOM_GEN_H

#include <random>
#include <chrono>

class random_gen
{
public:
    static unsigned seed1;
    static std::default_random_engine generator;
    static bool seeded;

    random_gen();

    ~random_gen();

    random_gen & operator=(const random_gen & in_random);

    random_gen(const random_gen & in_random);

    int randint(int min, int max);

    double randreal(double min, double max);
};
// class random_gen

random_gen::random_gen()
{
    if(seeded == false)
    {
        seed1 = std::chrono::system_clock::now().time_since_epoch().count();
        seeded = true;
        generator.seed(seed1);
    }
}


random_gen::~random_gen()
{
}


random_gen & random_gen::operator=(const random_gen & in_random)
{
    seed1 = in_random.seed1;
    generator.seed(seed1);
    return *this;
}



random_gen::random_gen(const random_gen & in_random)
{
    *this = in_random;
}




int random_gen::randint(int min, int max)
{
    int my_int = 0;

    std::uniform_int_distribution<int> distribution(min, max);
    my_int = distribution(generator);

    return my_int;
}



double random_gen::randreal(double min, double max)
{
    double my_double = 0;

    std::uniform_real_distribution<double> distribution(min, max);
    my_double = distribution(generator);
    return my_double;
}


// Declare statics
unsigned random_gen::seed1;
std::default_random_engine random_gen::generator;
bool random_gen::seeded = false;


#endif
