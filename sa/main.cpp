#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <random>
#include <vector>
#include <functional>
#include <iomanip>
#include <map>
#include <fstream>
#include <sstream>
#include "sa.h"
#define RASTRIGRIN_MIN -5.12
#define RASTRIGRIN_MAX 5.12


using namespace std;

random_device rdn;  //Will be used to obtain a seed for the random number engine
mt19937 mt_generator(rdn()); //Standard mersenne_twister_engine seeded with rd()

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


auto add_rand_val = [](auto p)
{
    normal_distribution<double> rand_val(0.0, 0.3);
    for (auto& e : p)
    {
        e = e + rand_val(mt_generator);
    }
    return p;
};



//auto eggholder = [](vector <double> v)
//{
//
//    double x = v.at(0), y = v.at(1);
//
//    double kk = abs(x/2 + (y + 47));
//    double ll = abs(x - (y + 47));
//
//    return -(y + 47)*sin(sqrt(kk)) - x*sin(ll);
//
//};
//
//auto eggholder_domain = [](vector <double> v)
//{
//    return (abs(v[0]) <= 512) && (abs(v[1]) <= 512);
//};
//
//uniform_real_distribution<> distrib_r(-512, 512);
//
//
//
//vector<double> eggholder_p0 =
//{
//    distrib_r(mt_generator),
//    distrib_r(mt_generator),
//};




auto rastrigin = [](vector <double> v)
{
    const int A = 10;

    int n = v.size();


    double rast = 0.0;
    double res_rast = 0.0;

    for(auto &e : v)

    {
        rast = rast + pow(e,2) - A*cos(2*M_PI*e);
    }
    res_rast = A*n + rast;
    return res_rast;
};

auto rastrigin_domain = [](vector<double> v)
{
    return (v[0] >= RASTRIGRIN_MIN) && (v[0] <= RASTRIGRIN_MAX);
};

uniform_real_distribution<> distrib_r(RASTRIGRIN_MIN, RASTRIGRIN_MAX);

vector<double> rastrigin_p0 =
{
    distrib_r(mt_generator),
    distrib_r(mt_generator),
};

using tempFunc = std::map<std::string, std::function<double(int)>>;

int main(int argc, char** argv)
{

    std::map<std::string, std::string> parameters =
    {
        {"iterations","100"},
        {"temp","10.0"},
        {"tempfunction","fast"}

    };

    for (auto [k, v] : args_to_map(std::vector<std::string>(argv, argv + argc)))
    {
        parameters[k] = v; // overwrite default parameters with the ones from CLI
    }

    int iterations = stoi(parameters["iterations"]);
    double t = stod(parameters["temp"]);


    auto T_fast = [&t](int k)
    {
        return t/k;
    };

    auto T_log = [&t] (int k)
    {
        return t/std::log10(1 + k);

    };


    tempFunc myTempFunc =
    {
        {"fast", T_fast},
        {"logarithmic", T_log}

    };

    // auto result= simulated_annealing(eggholder, eggholder_domain, eggholder_p0, iterations, add_rand_val, myTempFunc[parameters["tempfunction"]]);


    auto result= simulated_annealing(rastrigin,rastrigin_domain, rastrigin_p0, iterations, add_rand_val, myTempFunc[parameters["tempfunction"]]);
    cout << "f(" << result << ") = "<< " " << rastrigin(result) << endl;

    cout << endl;


    return 0;
}
