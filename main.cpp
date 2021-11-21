#include <iostream>
#include <vector>
#include<fstream>
#include<sstream>
#include<random>
#include <iterator>
#include <algorithm>
#include <math.h>
#include <chrono>
#include <experimental/random>
#include <map>
#include <stdlib.h>
#include "subset_sum_problem.hpp"
#include "hill_climbing.hpp"
#include "tabu.hpp"



std::map<std::string, std::string> args_to_map(std::vector<std::string> arguments)
{
    std::map<std::string, std::string> ret;
    std::string argname = "";
    for (auto param : arguments)
    {
        if ((param.size() > 2) && (param.substr(0, 2) == "--"))
        {
            argname = param.substr(2);
        }
        else
        {
            ret[argname] = param;
        }
    }
    return ret;
}


double targetSum;


auto next_point(my_set user_set, auto first, auto last)
{
    my_set subset;
    for ( ; first != last  ; ++first)
    {
        subset.push_back(user_set[(*first) - 1]);
    }
    return subset;
};

auto brute_force (my_set user_set,auto goal,int sum,std::ofstream &solFile)
{
    int stat_goal_function_calls = 0;
    const int n = user_set.size();
    std::vector<int> index_stack(n + 1, 0);

    int k = 0;

    int best_goal = 0;

    my_set res;
    std::cout << "Brute force" << std::endl;
    solFile << "Brute force" << std::endl;
    auto start = std::chrono::steady_clock::now();

    auto best_p = generate_random_subset_size(user_set, std::experimental::randint(0, (int) (user_set.size()-1)));

    double best_goal_val = goal(best_p, sum);
    //std::cout << "best" << best_goal_val << std::endl;
    while(1)
    {
        if (index_stack[k]<n)
        {
            index_stack[k+1] = index_stack[k] + 1;
            k++;
        }

        else
        {
            index_stack[k-1]++;
            k--;
        }

        if (k==0)
            break;

        stat_goal_function_calls++;

        res = next_point(user_set, begin(index_stack) + 1, begin(index_stack) + 1 + k);

//        if (stat_goal_function_calls == std::experimental::randint(0, (int) (user_set.size()-1)/2))
//        {
//            best_goal_val = goal(res,sum);
//        }
        //if (stat_goal_function_calls == 1 ) best_goal_val = goal(res,sum);
        double cost_value = goal(res,sum);

        if (cost_value < best_goal_val)
        {
            best_goal_val = cost_value;

            std::cout << res << " -> "  << "goal = " << best_goal_val << std::endl;
            solFile   << res << " -> "  << best_goal_val << std::endl;


        }


//        if (goal(res,sum) == best_goal)
//        {
//            std::cout << res << " -> "  << best_goal << std::endl ;
//            solFile   << res << " -> "  << best_goal << std::endl ;
//        };
    }

    auto finish = std::chrono::steady_clock::now();

    std::chrono::duration<double> duration = finish - start;

    solFile << "# count BF: " << stat_goal_function_calls << "; dt: " << duration.count() << std::endl;
    std::cout << "# count BF: " << stat_goal_function_calls << "; dt: " << duration.count() << std::endl;
}

int main(int argc, char** argv)
{


    std::map<std::string, std::string> parameters =
    {
        {"size", "10"},
        {"iterations","1000"},
        {"method", "brute_force"},
        {"print_result","false"},
        {"tabu_size","100"},
        {"inputfile",""},
        {"outputfile",""}
    };

    for (auto [k, v] : args_to_map(std::vector<std::string>(argv, argv + argc)))
    {
        parameters[k] = v; // overwrite default parameters with the ones from CLI
    }

    int iterations = stoi(parameters["iterations"]);

    int choice;

    if(!parameters["inputfile"].empty())
    {
        if(parameters["outputfile"].empty())
        {
            exit(EXIT_FAILURE);
        }


        std::cout << "Reading data from file : " << parameters["inputfile"] << std::endl;

        auto problem = read_problem(parameters["inputfile"]);

        std::ofstream solFile;
        solFile.open(parameters["outputfile"]);

        std::cout << "Problem : " << std::endl;
        std::cout << problem.problem_ssp_set << std::endl;
        std::cout << "Problem SUM: " << problem.sum << std::endl;
        std::cout << "\nSolutions : " << std::endl;

        if (parameters["method"] == "brute_force")
        {
            brute_force(problem.problem_ssp_set,&goal_function,problem.sum,solFile);
        }
        else if (parameters["method"] == "hill_climb")
        {
            hill_climb(problem.problem_ssp_set,&goal_function,problem.sum,iterations,solFile);
        }
        else if (parameters["method"] == "tabu")
        {

            tabu(problem.problem_ssp_set,&goal_function,problem.sum,iterations,stoi(parameters["tabu_size"]), solFile);
        }

        solFile.close();
    }

    else
    {

        std::cout << "How do you want data: "<< std::endl;
        std::cout << std::endl;
        std::cout << "1 -------> Read from file " << std::endl;
        std::cout << "2 -------> Random generate" << std::endl;
        std::cout << "3 -------> Input the problem to solve" << std::endl;

        std::cout << std::endl;
        std::cout << "Enter 1, 2 or 3: " << std::endl;

        std::cin >> choice;


        switch(choice)
        {
        case 1:
        {
            std::string input_file;
            std::string out_file;
            std::cout << "Enter the file name with problem"<< std::endl;

            std::cin >> input_file;

            std::cout << "Reading data from file" << std::endl;

            auto problem = read_problem(input_file);

            std::cout << "Problem : " << std::endl;

            std::cout << problem.problem_ssp_set << std::endl;

            std::cout << "\nProblem SUM: " << problem.sum << std::endl;

            std::cout << "Enter the name for file to save the results" << std::endl;

            std::cin >> out_file;


            std::cout << "\nSolutions : " << std::endl;


            std::ofstream solFile;
            solFile.open(out_file);

            brute_force(problem.problem_ssp_set,&goal_function,problem.sum,solFile);
            hill_climb(problem.problem_ssp_set,&goal_function,problem.sum,1000,solFile);
            tabu(problem.problem_ssp_set,&goal_function,problem.sum,iterations,stoi(parameters["tabu_size"]), solFile);

            solFile.close();

            break;
        }
        case 2:
        {

            std::cout << "Generating random problem\n" << std::endl;

            int n;
            std::cout << "Please input the size of set\n" << std::endl;
            std::cin >> n;
            auto problem = generate_random_problem(n);


            std::cout << "Problem : " << std::endl;
            std::cout << problem.problem_ssp_set << std::endl;
            std::cout << "\nProblem SUM: " << problem.sum << std::endl;
            std::cout << "\nSolutions : " << std::endl;
            std::ofstream null;

//            auto cut = generate_random_subset_size(problem.problem_ssp_set,problem.problem_ssp_set.size()/2);
//            std::cout << "cut " << cut << std::endl;
//
//            for(int i = 0; i<20; i++) {
//                 auto d = find_close_neighbour(problem.problem_ssp_set, cut);
//            std::cout << d << std::endl;
//
//            }

            brute_force(problem.problem_ssp_set,&goal_function,problem.sum,null);
            hill_climb(problem.problem_ssp_set,&goal_function,problem.sum,iterations,null);
            tabu(problem.problem_ssp_set,&goal_function,problem.sum,iterations,stoi(parameters["tabu_size"]), null);

            break;


        }
        case 3:
        {
            std::cin.clear();
            std::cin.sync();

            std::string set_num;
            int user_sum;


            std::cout << "Please input the set of numbers\n" << std::endl;

            getline(std::cin, set_num);

            std::stringstream input_parser_stream (set_num);
            std::cout << std::endl;

            std::cout << "Please input the target sum\n" << std::endl;
            std::cin >> user_sum;



            auto problem = generate_problem_from_input(input_parser_stream,user_sum);


            std::cout << "Problem : " << std::endl;
            std::cout << problem.problem_ssp_set << std::endl;
            std::cout << "\nProblem SUM: " << problem.sum << std::endl;
            std::cout << "\nSolutions : " << std::endl;
            std::ofstream null;

            brute_force(problem.problem_ssp_set,&goal_function,problem.sum,null);
            hill_climb(problem.problem_ssp_set,&goal_function,problem.sum,pow(2, problem.problem_ssp_set.size()),null);
            tabu(problem.problem_ssp_set,&goal_function,problem.sum,iterations,stoi(parameters["tabu_size"]), null);

            break;

        }

        default:
            std::cout << "Put correct choice" << std::endl;
            break;

        }
    }


    return 0;
}
