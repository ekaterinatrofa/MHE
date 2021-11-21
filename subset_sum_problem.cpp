#include <vector>
#include <iostream>
#include <numeric>
#include <random>
#include <algorithm>
#include<fstream>
#include<sstream>
#include <experimental/random>
#include <iterator>

#include "subset_sum_problem.hpp"


using namespace std;

struct Problem;

double goal_function(my_set subsets, double targetSum)
{
    int temp_sum = std::accumulate(subsets.begin(),subsets.end(),0);

    double difference = abs(temp_sum - targetSum);

    return difference;
}


mt19937 rand_gen(time(nullptr));

my_set generate_random_set(int n)
{

    uniform_int_distribution<int> distr(1, n*2);

    my_set random_problem_set;

    vector<double>::iterator it;

    for (int i = 0; i < n; i++)
    {

        double x = distr(rand_gen);

        it = find(random_problem_set.begin(),random_problem_set.end(),x);

        if(it==random_problem_set.end())
        {

            random_problem_set.push_back(x);
        }
        else
        {

            i--;
        }
    }
    return random_problem_set;
}


my_set generate_random_subset(my_set random_set)
{

    my_set random_subset;

    mt19937 rand_gen(time(nullptr));
    uniform_int_distribution<int> distr(1, random_set.size()-1);

    int x = distr(rand_gen);
//    cout << x << endl;

    std::sample(random_set.begin(), random_set.end(), std::back_inserter(random_subset),
                x, std::mt19937{std::random_device{}()});

    return random_subset;
}



my_set generate_random_subset_size (my_set random_set, int size_set)
{

    my_set random_subset;

    mt19937 rand_gen(time(nullptr));
    uniform_int_distribution<int> distr(1, random_set.size());


    std::sample(random_set.begin(), random_set.end(), std::back_inserter(random_subset),
                size_set, std::mt19937{std::random_device{}()});



    return random_subset;


}

Problem generate_random_problem(int n)
{

    my_set problem_set = generate_random_set(n);

    my_set problem_sub_set = generate_random_subset(problem_set);

    int temp_sum = std::accumulate(problem_sub_set.begin(),problem_sub_set.end(),0);

    Problem my_problem;
    my_problem.sum = temp_sum;
    my_problem.problem_ssp_set = problem_set;


    return my_problem;
}


Problem read_problem(std::string filename)
{
    std::ifstream input_file(filename);
    Problem out;
    input_file >> out.sum;
    while (!input_file.eof())
    {
        double val;
        input_file >> val;
        out.problem_ssp_set.push_back(val);
    }
    return out;
}


Problem generate_problem_from_input(stringstream &input_set,int sum)
{
    int number;
    my_set myNumbers;
    while ( input_set >> number )
        myNumbers.push_back( number );

    Problem my_problem;
    my_problem.sum = sum;
    my_problem.problem_ssp_set = myNumbers;


    return my_problem;
}


my_set find_close_neighbour(my_set entry_set, my_set work_point)
{

    my_set new_subset;

    std::copy ( work_point.begin(),work_point.end(), std::back_inserter(new_subset));


    //cout <<"new " << new_subset << endl;

    mt19937 rand_gen(time(nullptr));

    uniform_int_distribution<int> distr(0, entry_set.size()-1);

    uniform_int_distribution<int> distr1(0, entry_set.size()-2);
    int v_size = entry_set.size();

    //int first = distr(rand_gen);
    int first = std::experimental::randint(0, (v_size - 1 ));
    //int second = distr1(rand_gen);
    int second =  std::experimental::randint(0, (v_size - 2));

    auto first_item = entry_set[first];

    auto second_item = entry_set[second];

    if(std::find(work_point.begin(), work_point.end(), first_item)!=work_point.end())
    {


       // new_subset.remove(new_subset.begin(),new_subset.end(),first_item);
      // new_subset.erase(std::remove(new_subset.begin(), new_subset.end(), first_item), new_subset.end());
       new_subset.erase(std::remove(new_subset.begin(), new_subset.end(), first_item));

//      std::vector <double>::iterator position = std::find(new_subset.begin(), new_subset.end(), first_item);
//      if(position != new_subset.end()) {
//
//        new_subset.erase(position);
//      }
    }
    else
    {

        new_subset.push_back(first_item);
    }

    if((std::find(work_point.begin(), work_point.end(), second_item)!=work_point.end())
       && (std::find(new_subset.begin(), new_subset.end(), second_item)!=new_subset.end()))
{

    if(std::experimental::randint(1, 2)==1)
        {

            //new_subset.remove(new_subset.begin(),new_subset.end(),second_item);
            //new_subset.erase(std::remove(new_subset.begin(), new_subset.end(), second_item), new_subset.end());
            new_subset.erase(std::remove(new_subset.begin(), new_subset.end(), second_item));
//           std::vector <double>::iterator position = std::find(new_subset.begin(), new_subset.end(), second_item);
//      if(position != new_subset.end()) {
//
//        new_subset.erase(position);
//      }

        }

}
//    else
//    {
//
//        if(std::experimental::randint(1, 2)==1)
//        {
//
//            new_subset.push_back(second_item);
//        }
//
//
//    }


    return new_subset;
}


ostream& operator<<(ostream& o, const my_set problem_ssp_set)
{
    o << "{";
    for(int i = 0; i < problem_ssp_set.size(); i++)
    {
        o << problem_ssp_set[i] << ",";
    }
    o << "}";
    return o;
}






