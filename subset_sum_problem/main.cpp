#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
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
#include "ga.hpp"
#include <boost/any.hpp>
#include <any>
#include <typeinfo>

using chromosome_t = vector<int>;
using selectionMap = std::map<std::string, std::function<chromosome_t(vector<chromosome_t> &, struct Problem)>>;
using crossoverMap = std::map<std::string, std::function<pair<chromosome_t, chromosome_t>(chromosome_t &, chromosome_t &, double)>>;


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

    std::cout << best_p << " -> "  << "result: "<< best_goal_val << std::endl ;
}

///ga

auto init_pop = [](int s, int n) -> vector<chromosome_t>
{
    vector<chromosome_t> ret;

    chromosome_t numbers;

    for (int i = 0; i < s; i++)
    {

        for (int j = 0; j < n; j++)
        {

            int x = uniform_int_distribution<int>(0, 1)(randgen);
            numbers.push_back(x);

        }

        ret.push_back(numbers);
        numbers.clear();
    }
    return ret;
};


/// fitness

auto fitness = [](auto chromosome_t, auto Problem)->double
{
    my_set work_set;
    for (int i = 0 ; i < chromosome_t.size(); ++i)
    {

        if(chromosome_t.at(i) == 1)
        {
            work_set.push_back(Problem.problem_ssp_set[i]);
        }
    }


    auto fit_goal = goal_function(work_set,Problem.sum);

    return 1.0/(1+fit_goal);

};


// Tournament selection

selection_f_t select = [](vector<chromosome_t> &population,auto Problem) -> chromosome_t
{

    uniform_int_distribution<int> dist(0, population.size() - 1);

    auto s1 = population.at(dist(randgen));
    auto s2 = population.at(dist(randgen));


    if ((fitness(s1,Problem )) > (fitness(s2,Problem)))
    {
        return s1;
    }

    return s2;
};

//Roulette selection
selection_f_t select_roulette = [](vector<chromosome_t> &population,auto Problem) -> chromosome_t
{
    std::vector<double> fs ;


    for(int i = 0; i < population.size(); i++)
    {
        fs.push_back(fitness(population[i], Problem));
    }
//    std::cout << fs << std:: endl;
    double sum_fit = accumulate(fs.begin(), fs.end(), 0.0);
//    std::cout << sum_fit << std:: endl;
    // double max_fs = *max_element(fs.begin(), fs.end());
    // double min_fs = *min_element(fs.begin(), fs.end());

    double u = 0.0;
    u = uniform_real_distribution<double>(0.0, sum_fit)(randgen);
    // std::cout <<"rand = "<< u << std:: endl;;


    for (int i = 0; i < population.size(); i++)
    {
        u -= fitness(population[i], Problem);
        //u -= (t - fitness(population[i], Problem));
        if (u < 0)
        {
//            std::cout <<"best pop = "<< i << std:: endl;
            return population[i];
            break;
        }
    }

    fs.erase(fs.begin(),fs.end());

    return population[0];
};



crossover_f_t crossover_one_point = [](auto &a, auto &b, double p_crossover)
{
    using namespace std;


    uniform_int_distribution<int>dist(0, a.size() - 1);

    double u = uniform_real_distribution<double>(0.0, 1.0)(randgen);
    if(u < p_crossover)
    {
        int cross_point = dist(randgen);
        auto new_a = a;
        auto new_b = b;

        for (int i = cross_point; i < (int)a.size(); i++)
        {
            new_a[i] = b[i];
            new_b[i] = a[i];
        }
        return pair<chromosome_t, chromosome_t>(new_a, new_b);

    }
    return pair<chromosome_t, chromosome_t>(a, b);

};

crossover_f_t crossover_two_point = [](auto &a, auto &b, double p_crossover)
{
    using namespace std;


    uniform_int_distribution<int>dist(0, a.size() - 1);

    double u = uniform_real_distribution<double>(0.0, 1.0)(randgen);

    if(u < p_crossover)
    {

        int cross_point_a = uniform_int_distribution<int>(0, a.size() - 1)(randgen);
        int cross_point_b = uniform_int_distribution<int>(0, a.size() - 1)(randgen);

        if  (cross_point_a > cross_point_b)
        {

            swap(cross_point_a, cross_point_b);
        }

        auto new_a = a;
        auto new_b = b;

        for (int i = cross_point_a; i < cross_point_b; i++)
        {
            new_a[i] = b[i];
            new_b[i] = a[i];
        }
        return pair<chromosome_t, chromosome_t>(new_a, new_b);


    }

    return pair<chromosome_t, chromosome_t>(a, b);

};


mutation_f_t mutation_swap = [](chromosome_t c, double pm) -> chromosome_t
{
    static uniform_real_distribution<double> dist(0.0, 1.0);
    for (int i = 0; i < c.size(); ++i)
    {

        if (dist(randgen) < pm)
        {

            if (c[i] == 0)
            {
                c[i] = 1;
            }
            else
            {
                c[i] = 0;
            }
        }
    }
    return c;

};

auto term_avg = [&fitness](std::vector<chromosome_t> pop, int iteration,auto Problem)
{
    double avg = accumulate(pop.begin(), pop.end(), (double)0.0,
                            [&](double a, chromosome_t b)
    {
        return a + fitness(b,Problem);
    })/(double)pop.size();

//std::cout <<"avg "<< avg << std::endl;

    return avg > 0.5;
};


auto term_stddev = [&fitness](std::vector<chromosome_t> pop, int iteration, auto Problem)
{

    double stddev = 0;
    double avg = accumulate(pop.begin(), pop.end(), (double)0.0,
                            [&](double a, chromosome_t b)
    {
        return a + fitness(b,Problem);
    })/(double)pop.size();

    double sum = accumulate(pop.begin(), pop.end(), (double)0.0,
                            [&](double a, chromosome_t b)
    {
        return a + pow((fitness(b, Problem) - avg), 2);
    });

    stddev = sqrt(sum / ((double)pop.size()- 1));

    //cout << iteration << " " << avg << " " << sum << " " << stddev;

    cout << endl;
    return stddev > 0.3;
};


auto term_iteration = [](auto pop, int i, auto Problem)


{
    //std::cout<< "i = "<< i<< std:: endl;
    return i < 100;
};


auto print_stats = [&fitness](auto comment, auto pop,auto Problem)
{

    auto best_of_all = *std::max_element(pop.begin(), pop.end(), [&](auto &a, auto &b)
    {
        return fitness(a,Problem) < fitness(b, Problem);
    });

    auto worse_of_all = *std::max_element(pop.begin(), pop.end(), [&](auto &a, auto &b)
    {
        return fitness(a,Problem) > fitness(b,Problem);
    });
    my_set result;
    for (int i = 0 ; i< best_of_all.size(); ++i)
    {
        if(best_of_all[i] == 1)
        {
            result.push_back(Problem.problem_ssp_set.at(i));

        }
    }

    my_set res;
    for (int i = 0 ; i< worse_of_all.size(); ++i)
    {
        if(worse_of_all[i] == 1)
        {
            res.push_back(Problem.problem_ssp_set.at(i));

        }
    }
//    auto fit = fitness1(best_of_all);
//    cout << fit << endl;
    cout << "# [" << comment << "] best: " << fitness(best_of_all,Problem) << " result set: " << result << " goal: " <<  goal_function(result,Problem.sum)  << endl;
    cout << "# [" << comment << "] worse: " << fitness(worse_of_all,Problem) << " result set: " << res << " goal: " <<  goal_function(res,Problem.sum)  << endl;

};




//using terminationMap = std::map<std::string, std::function<bool(*fitness))(vector<chromosome_t>,int,struct Problem)>>;
using terminationMap = std::map<std::string, std::any>;



int main(int argc, char** argv)
{


    std::map<std::string, std::string> parameters =
    {
        {"size", "0" },
        {"iterations","10000"},
        {"method", "brute_force"},
        {"print_result","false"},
        {"tabu_size","1000"},
        {"inputfile",""},
        {"outputfile",""},
        {"mutation_prob", "0.01"},
        {"crossover_prob", "1.0"},
        {"selection", "tournament_selection"},
        {"crossover", "one"},
        {"population_size", "10"},
        {"termination", "iteration"}


    };

    for (auto [k, v] : args_to_map(std::vector<std::string>(argv, argv + argc)))
    {
        parameters[k] = v; // overwrite default parameters with the ones from CLI
    }

    int iterations = stoi(parameters["iterations"]);
    int problem_size = stoi(parameters["size"]);
    int tabu_size = stoi(parameters["tabu_size"]);


    selectionMap mySelect =
    {
        {"tournament_selection", select},
        {"roulette_selection", select_roulette}
    };


    crossoverMap myCrossover =
    {
        {"one", crossover_one_point},
        {"two", crossover_two_point}
    };



    terminationMap myTerm =
    {

        {"iteration", term_iteration},
        {"deviation", term_stddev},
        {"average", term_avg}
    };




    int choice;

    if (parameters["method"] == "ga")
    {

        double mutation_prob = stod(parameters["mutation_prob"]);
        double crossover_prob = stod(parameters["crossover_prob"]);
        auto selection = mySelect[parameters["selection"]];
        auto crossover = myCrossover[parameters["crossover"]];
        auto pop_size = stoi(parameters["population_size"]);


//        std::cout << "Generating random problem\n" << std::endl;

        int n = 20;
        //auto problem = generate_random_problem(n);
        auto problem = read_problem(parameters["inputfile"]);


        std::cout << "Problem : " << std::endl;
        std::cout << problem.problem_ssp_set << std::endl;
        std::cout << "\nProblem SUM: " << problem.sum << std::endl;
        std::cout << "\nSolutions : " << std::endl;

        auto chromosome_length = problem.problem_ssp_set.size();

        auto initial_pop = init_pop(pop_size,chromosome_length);



        if (parameters["termination"] == "iteration")
        {

            auto term = std::any_cast<decltype(term_iteration)>(myTerm["iteration"]);
            auto ga = genetic_algorithm(initial_pop, problem, term, selection, crossover, mutation_swap,crossover_prob, mutation_prob);
            print_stats("result ", ga,problem);

        }
        else if (parameters["termination"] == "deviation")
        {

            auto term = std::any_cast<decltype(term_stddev)>(myTerm["deviation"]);
            auto ga = genetic_algorithm(initial_pop, problem, term, selection, crossover, mutation_swap,crossover_prob, mutation_prob);
            print_stats("result ", ga,problem);

        }
        else if (parameters["termination"] == "average")
        {

            auto term = std::any_cast<decltype(term_avg)>(myTerm["average"]);
            auto ga = genetic_algorithm(initial_pop, problem, term, selection, crossover, mutation_swap,crossover_prob, mutation_prob);
            print_stats("result ", ga,problem);

        }

        exit(0);

    }

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
            hill_climb(problem.problem_ssp_set,&goal_function,problem.sum,iterations, solFile);
        }
        else if (parameters["method"] == "tabu")
        {

            tabu(problem.problem_ssp_set,&goal_function,problem.sum,iterations,tabu_size, solFile);
        }

        solFile.close();
    }
    else if(problem_size != 0)
    {


        auto problem = generate_random_problem(problem_size);


        std::cout << "Problem : " << std::endl;
        std::cout << problem.problem_ssp_set << std::endl;
        std::cout << "\nProblem SUM: " << problem.sum << std::endl;
        std::cout << "\nSolutions : " << std::endl;
        std::ofstream null;
        if (parameters["method"] == "brute_force")
        {
            brute_force(problem.problem_ssp_set,&goal_function,problem.sum,null);
        }
        else if (parameters["method"] == "hill_climb")
        {
            hill_climb(problem.problem_ssp_set,&goal_function,problem.sum,iterations, null);
        }
        else if (parameters["method"] == "tabu")
        {

            tabu(problem.problem_ssp_set,&goal_function,problem.sum,iterations,tabu_size, null);
        }

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
            hill_climb(problem.problem_ssp_set,&goal_function,problem.sum,iterations, solFile);
            tabu(problem.problem_ssp_set,&goal_function,problem.sum,iterations,tabu_size, solFile);

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
            std::cout << "-----" << std::endl;
            hill_climb(problem.problem_ssp_set,&goal_function,problem.sum,iterations,  null);
            std::cout << "-----" << std::endl;
            tabu(problem.problem_ssp_set,&goal_function,problem.sum,iterations,tabu_size, null);

            std::cout << "xxxxxxxxxxxxxxxxxxxxxxxxxx"<< std::endl;

            auto chromosome_length = problem.problem_ssp_set.size();

            auto initial_pop = init_pop(10,chromosome_length);

            int term_condition;

            cout << "Choose termination condition, 1 - number of iterations, 2 - Average fitness value "<< endl;
            cin >> term_condition;

            if(term_condition == 1)
            {


//                auto kate = mySelect["select"];
//                auto dupa = myCrossover["one"];
                auto ga = genetic_algorithm(initial_pop, problem, term_iteration, select, crossover_one_point, mutation_swap,0.9, 0.01);

//            for(auto e: ga) {
//
//
//                for(auto x: e){
//
//                    std::cout<< x;
//                }
//                std::cout << " " << std::endl;
//            }

                print_stats("result ", ga,problem);

            }
            else if(term_condition == 2)
            {
                auto ga = genetic_algorithm(initial_pop, problem, term_stddev, select, crossover_one_point, mutation_swap,0.8, 0.01);

//            for(auto e: ga) {
//
//
//                for(auto x: e){
//
//                    std::cout<< x;
//                }
//                std::cout << " " << std::endl;
//            }}

                print_stats("result ", ga,problem);
            }

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
            hill_climb(problem.problem_ssp_set,&goal_function,problem.sum,pow(2, problem.problem_ssp_set.size()), null);
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
