#include "sa.h"
#include <vector>
#include <functional>
#include <iostream>
#include <string>
#include <random>
#include <iomanip>
#include <fstream>

random_device rd;
mt19937 gen(rd());

std::string gnuplotrun()
{
    FILE* fp = popen("gnuplot script_sa.plt", "r");
    std::string stdout_plot = "";
    int ch = 0;
    while ((ch = fgetc(fp)) != EOF)
    {
        stdout_plot = stdout_plot + (char)ch;
    }
    int status = pclose(fp);
    if (status != 0)
    {
        throw std::invalid_argument("error generating chart");
    }

    return stdout_plot;
}

/**

The simulated annealing

**/

vector<double> simulated_annealing(
    function<double(vector<double>)> f,             //goal function
    function<bool(vector<double>)> f_domain,
    vector<double> p0,
    int iterations,
    function<vector<double>(vector<double>)> N,
    function<double(int)> T
    //temperature
)
{
    auto s_current = p0;
    auto s_global_best = p0;


    uniform_real_distribution<> u_k(0.0, 1.0);

    if (!f_domain(s_current)) throw std::invalid_argument("The p0 point must be in domain");

    std::ofstream saFile;
    saFile.open("log_2000_10.txt");
    for (int k = 0; k < iterations; k++)
    {
        auto s_next = N(s_current);

        if (f_domain(s_next))
        {
            if (f(s_next) < f(s_current))
            {
                s_current = s_next;
            }
            else
            {
                double u = u_k(gen);
                if (u < exp(-abs(f(s_next) - f(s_current)) / T(k)))
                {
                    s_current = s_next;
                }
            }
        }
        if (f(s_current) < f(s_global_best))
        {
            s_global_best = s_current;
        }

        std::cout << k << " " << f(s_current) << endl;

//        std::cout << s_current << " " << f(s_current) << endl;



        saFile << k <<  '\t';
        saFile <<f(s_current);
        saFile << '\n';

    }
    saFile.close();
    cout << gnuplotrun() << endl;
    return s_global_best;
}

ostream& operator<<(ostream& o, vector<double> v)
{
    for (auto e : v)
    {
        o << std::fixed << std::setprecision(5) << " " << e;
    }
    return o;
}
