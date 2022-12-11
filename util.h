#ifndef UTIL_H  
#define UTIL_H

namespace Util
{
    std::string now();
    std::string to_string(double, int);
    std::vector<std::string> parse_args(int, char**);
    std::vector<std::vector<double>> normalize(std::vector<std::vector<double>>);
    std::vector<std::vector<double>> combine(std::vector<std::vector<double>>, std::vector<std::vector<double>>, double);
}

#endif