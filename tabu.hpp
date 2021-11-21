#ifndef TABU_HPP_INCLUDED
#define TABU_HPP_INCLUDED

#include <algorithm>
#include <chrono>
#include <functional>
#include <vector>
#include <numeric>


auto tabu =[](my_set user_set, auto goal, double sum, int N,
              int tabu_size, std::ofstream &solFile = std::cout)
{
    using namespace std;

    vector<vector<double>> tabu_list;

    int size_set = user_set.size()/2;

    auto best_p = generate_random_subset_size(user_set, size_set);

    double best_goal_val = goal(best_p,sum);

    auto is_in_taboo = [&](auto e)
    {

        return count(tabu_list.begin(),tabu_list.end(),e);
    };



    auto shrink_taboo = [&]()
    {
        if (tabu_list.size() > tabu_size)
        {
           // cout << tabu_list.size() << endl;
            tabu_list.erase(tabu_list.begin());
          //  cout << tabu_list.size() << endl;
        }
    };





    auto start = chrono::steady_clock::now();

    int attempt = 0;
    auto p = best_p; // current work point
    cout << "Tabu" << endl;
    solFile << "Tabu" << std::endl;

    for (int i = 0; i < N; i++)
    {
        attempt++;

        shrink_taboo();
       // std::cout << "best_p" << best_p << std::endl ;
        auto p = find_close_neighbour(user_set, best_p);

        auto cost_value = goal(p,sum);

         if (cost_value < best_goal_val)
        {

            best_goal_val = cost_value;
            best_p = p;

            std::cout << best_p << " -> "  << "goal = "<< best_goal_val << std::endl ;
            solFile   << best_p << " -> "  << best_goal_val << std::endl ;

        }

       // std::cout << is_in_taboo(p) << std::endl;
        if ((cost_value > best_goal_val) && (is_in_taboo(p)== 0))
        {
         //  std::cout << "Adding to vector" << std::endl ;
            tabu_list.push_back(best_p);
            best_p = p;

          //  std::cout <<"Neighbour "<< p << std::endl ;
          //  std::cout << "----" << std::endl ;


        }



    }


    auto finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;
    std::cout << "# count TS: " << attempt << "; dt: " << duration.count() << std::endl;
    solFile << "# count TS: " << attempt << "; dt: " << duration.count() << std::endl;

};

#endif // TABU_HPP_INCLUDED
