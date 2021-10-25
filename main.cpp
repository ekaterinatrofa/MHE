#include <iostream>
#include <vector>
#include<fstream>
#include<sstream>
#include<random>
#include <iterator>
#include <algorithm>
#include <math.h>

using namespace std;


using my_set = std::vector<double>;
using my_subsets = std::vector<std::vector<double>>;

double targetSum;

struct Problem
{
    my_set problem_ssp_set;
    int sum;
};

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

mt19937 rand_gen(time(nullptr));

/**

function for random generating set without duplicates numbers

**/

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

/**

function that derives solution from randomly generated set
working point

**/


my_set generate_random_subset (my_set random_set)
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

Problem generate_problem_from_input(my_set input_set)
{

    my_set problem_sub_set = generate_random_subset(input_set);
    int temp_sum = std::accumulate(problem_sub_set.begin(),problem_sub_set.end(),0);
    Problem my_problem;
    my_problem.sum = temp_sum;
    my_problem.problem_ssp_set = input_set;


    return my_problem;
}


//vector<vector<double>> findAllSubsets(my_set ssp_problem)
//{
//
//
//    my_subsets subsets;
//
//
//    for(int i = 0; i < (1 << ssp_problem.size()); i++)
//    {
//
//        my_set current_subset;
//
//cout << " I" << i << endl;
//
//        for(int j = 0; j < ssp_problem.size(); j++)
//        {
//            if( (i & (1 << j)))
//            {
//                current_subset.push_back(ssp_problem[j]);
//
//            }
//        }
//        subsets.push_back(current_subset);
//    }
//
//    return subsets;
//}

my_set findAllPossibleWorkPoints(my_set ssp_problem, int counter)
{

    my_set subset;

    for (int i=0; i<ssp_problem.size(); i++)
    {

        int index=1<<i;

        if((index&counter)>0)
        {

            subset.push_back(ssp_problem[i]);

        }

    }

    return subset;
}


double goal_function(my_set subsets, double targetSum)
{
    int temp_sum = std::accumulate(subsets.begin(),subsets.end(),0);

    double difference = abs(temp_sum - targetSum);

    return difference;
}

//void printPowerset (my_set random_set)
//{
//    auto print_set = [&random_set](auto first, auto last) -> ostream&
//    {
//        cout << '(';
//        auto sep = "";
//        for ( ; first != last  ; ++first, sep = ",")
//        {
//            cout << sep << random_set[(*first) - 1];
//        }
//        return cout << ')';
//    };
//
//    const int n = random_set.size();
//    std::vector<int> index_stack(n + 1, 0);
//    int k = 0;
//
//
//    while(1){
//        if (index_stack[k]<n){
//            index_stack[k+1] = index_stack[k] + 1;
//            k++;
//        }
//
//        else{
//            index_stack[k-1]++;
//            k--;
//        }
//
//        if (k==0)
//            break;
//
//        print_set(begin(index_stack) + 1, begin(index_stack) + 1 + k);
//    }
//    print_set(begin(index_stack), begin(index_stack)) << endl;
//}



my_set load_problem(string fname)
{
    ifstream inputfile(fname);
    my_set result;
    std::string line;
    while (std::getline(inputfile, line))
    {

        std::istringstream iss(line);
        double value;
        while (!iss.eof())
        {
            iss >> value;
            if (value < 0) throw std::invalid_argument ("Invalid input data! All inputs should be positive ");
            result.push_back(value);
        }
    }
    return result;
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

int main()
{

    int choice;


    cout << "How do you want data: "<< endl;
    cout << endl;
    cout << "1 -------> Read from file " << endl;
    cout << "2 -------> Random generate" << endl;

    cout << endl;
    cout << "Enter 1 or 2: " << endl;

    cin >> choice;


    switch(choice)
    {
    case 1:
    {

        cout << "Reading data from file" << endl;

        //auto input = load_problem("ssp.txt");
        auto problem = read_problem("ssp.txt");

        // auto problem = generate_problem_from_input(pr);
        cout << "Problem : " << endl;
        cout << problem.problem_ssp_set << endl;

        cout << "\nProblem SUM: " << problem.sum << endl;

        for(int i = 0; i < pow(2,problem.problem_ssp_set.size()); i++)
        {

            auto s_set = findAllPossibleWorkPoints(problem.problem_ssp_set,i);
            auto solution = goal_function(s_set,problem.sum);
            cout<< s_set;


            cout << " -> " <<solution << " ";
            cout << endl;

        }

        break;
    }
    case 2:
    {

        cout << "Generating random problem\n" << endl;

        int n;
        cout << "Please input the size of set\n" << endl;
        cin >> n;
        auto problem = generate_random_problem(n);


        cout << "Problem : " << endl;
        cout << problem.problem_ssp_set << endl;
        cout << "\nProblem SUM: " << problem.sum << endl;
        cout << "\nSolutions : " << endl;


        for(int i = 0; i < pow(2,problem.problem_ssp_set.size()); i++)
        {

            auto s_set = findAllPossibleWorkPoints(problem.problem_ssp_set,i);
            auto solution = goal_function(s_set,problem.sum);
            cout<< s_set;


            cout << " -> " <<solution << " ";
            cout << endl;

        }
        break;


    }

    default:
        cout << "Put correct choice" << endl;
        break;

    }



    return 0;
}
