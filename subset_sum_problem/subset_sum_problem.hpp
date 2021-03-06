#ifndef SUBSET_SUM_PROBLEM_HPP_INCLUDED
#define SUBSET_SUM_PROBLEM_HPP_INCLUDED

#include <iostream>
#include<iterator>


/**
 * set representing problem
 * */

using my_set = std::vector<double>;


/**
 * structure for Subset Sum Problem
 * */

struct Problem
{
    my_set problem_ssp_set;
    int sum;
};


/**
 * generates cost function for Subset Sum Problem
 * */

double goal_function(my_set subsets, double targetSum);


/**
 * function for random generating set without duplicates numbers
 * */

my_set generate_random_set(int n);


/**
 *function that derives solution from randomly generated set working point
 * */


my_set generate_random_subset (my_set random_set);


my_set generate_random_subset_size (my_set random_set, int size_set);


/**
 * function for generating random problem
 * */

Problem generate_random_problem(int n);


/**
 * function for reading problem from file
 * */

Problem read_problem(std::string filename);

/**
 * function for reading problem from user's input
 * */

Problem generate_problem_from_input(std::stringstream &input_set,int sum);





my_set find_close_neighbour(my_set work_point, my_set user_set);


std::ostream& operator<<(std::ostream& o, const my_set problem_ssp_set);

#endif // SUBSET_SUM_PROBLEM_HPP_INCLUDED
