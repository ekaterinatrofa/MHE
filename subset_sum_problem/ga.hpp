#ifndef GA_HPP_INCLUDED
#define GA_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <functional>
#include <chrono>
#include <omp.h>



using namespace std;

using chromosome_t = vector<int>;
using selection_f_t = function<chromosome_t(vector<chromosome_t> &, struct Problem)>;
using crossover_f_t = function<pair<chromosome_t, chromosome_t>(chromosome_t &, chromosome_t &, double)>;
using mutation_f_t = function<chromosome_t(chromosome_t, double)>;

random_device rd;
mt19937 randgen(rd());


auto genetic_algorithm = [](
                             auto population,
                             auto Problem,
                             auto term_condition,
                             selection_f_t select,
                             crossover_f_t crossover,
                             mutation_f_t mutation,
                             double p_crossover,
                             double p_mutation)
{
    auto start = chrono::steady_clock::now();

    for (int iteration = 0; term_condition(population, iteration,Problem); iteration++)
    {

        vector<chromosome_t> new_pop(population.size());
        #pragma omp parallel for

        for (int c = 0; c < (population.size() / 2); c++)
        {

            chromosome_t parent1 = select(population, Problem);
            chromosome_t parent2 = select(population, Problem);
            auto [child1, child2] = crossover(parent1, parent2,p_crossover);
            child1 = mutation(child1, p_mutation);
            child2 = mutation(child2, p_mutation);

            new_pop[c*2  ] = child1;

            new_pop[c*2+1] = child2;
        }
        population = new_pop;
    }

    auto finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;
    std::cout <<  "dt: " << duration.count() << std::endl;
    return population;
};

















#endif // GA_HPP_INCLUDED
