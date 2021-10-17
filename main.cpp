#include <iostream>
#include <vector>
#include<fstream>
#include<sstream>

using namespace std;


using my_set = std::vector<double>;
using my_subsets = std::vector<std::vector<double>>;

double targetSum;


vector<vector<double>> findAllSubsets(my_set ssp_problem)
{
    my_subsets subsets;

    for(int i = 0; i < (1 << ssp_problem.size()); i++)
    {

        my_set current_subset;

        for(int j = 0; j < ssp_problem.size(); j++)
        {
            if( (i & (1 << j)))
            {
                current_subset.push_back(ssp_problem[j]);
            }
        }
        subsets.push_back(current_subset);
    }

    return subsets;
}

double goal_function(my_set subsets,double targetSum)
{
    int temp_sum = 0;

    for(auto elem: subsets)
    {

        temp_sum += elem;

    }

    double difference = abs(temp_sum - targetSum);

//    if(difference == 0.0)
//    {
//        return difference;
//
//    }


    return difference;
}

void print_subsets(my_set subsets)
{
    cout << "{";
    for(int i = 0; i < subsets.size(); i++)
    {
        cout << subsets[i] << ",";
    }
    cout << "}";

}

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

void print_set(my_set f_set)
{
    cout << "Input data from file: " << endl;

    for (auto row : f_set)
    {
        cout << row << " ";
    }
    cout<< endl;
}

int main()
{

    auto problem = load_problem("ssp.txt");

    print_set(problem);
    cout<< endl;

    double required_sum = 5.0;

    vector<vector<double>>subsets = findAllSubsets(problem);

    cout << "Results" << endl;
    cout<< endl;


    for(int i = 0; i < subsets.size(); i++)
    {
        auto solution = goal_function(subsets[i],required_sum);

//        if(solution==0)
//        {
            print_subsets(subsets[i]);
            cout << " -> " <<solution << " ";
            cout << endl;
//        }
    }


    return 0;
}
