#ifndef UTIL_H  
#define UTIL_H

namespace Util
{
    std::string now();
    std::string to_string(double, int);
    std::vector<std::string> parse_args(int, char**);
    std::vector<double> parse_gammas(std::string);
    std::vector<std::vector<double>> normalize(std::vector<std::vector<double>>);
    std::vector<std::vector<double>> combine(std::vector<std::vector<double>>, std::vector<std::vector<double>>, double);
    std::vector<std::vector<double>> bridge(std::vector<std::vector<unsigned>>, std::vector<std::vector<unsigned>>, std::vector<std::vector<double>>, double);
    std::vector<std::string> merge_labels(std::vector<std::vector<double>>, std::vector<std::string>, std::vector<std::string>, double);
    std::vector<std::vector<double>> merge(std::vector<std::vector<unsigned>>, std::vector<std::vector<unsigned>>, std::vector<std::vector<double>>, std::vector<std::string>, std::vector<std::string>, std::vector<std::string>, double);
}

#endif