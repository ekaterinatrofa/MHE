#ifndef HILL_CLIMBING_HPP_INCLUDED
#define HILL_CLIMBING_HPP_INCLUDED

#include <chrono>


auto hill_climb = [](my_set user_set,
                     auto goal,
                     double sum,
                     int N,
                     std::ofstream &solFile = std::cout)
{
    using namespace std;

    int size_set = user_set.size()/2;

    auto best_p = generate_random_subset_size(user_set, size_set);

    double best_goal_val = goal(best_p,sum);

    auto start = chrono::steady_clock::now();

    int attempt = 0;
    auto p = best_p; // current work point
    cout << "HillClimbing" << endl;
    solFile << "HillClimbing" << std::endl;

    for (int i = 0; i < N; i++)
    {
        attempt++;

        auto p = find_close_neighbour(user_set, best_p);

        auto cost_value = goal(p,sum);

        if (cost_value < best_goal_val)
        {

            best_goal_val = cost_value;
            best_p = p;

            std::cout << best_p << " -> "  << "goal = "<< best_goal_val << std::endl ;
            solFile   << best_p << " -> "  << best_goal_val << std::endl ;

        }

    }


    auto finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;
    std::cout << "# count HC: " << attempt << "; dt: " << duration.count() << std::endl;
    solFile << "# count HC: " << attempt << "; dt: " << duration.count() << std::endl;

};


#endif // HILL_CLIMBING_HPP_INCLUDED
