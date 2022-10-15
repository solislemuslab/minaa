#ifndef UTIL_H  
#define UTIL_H

namespace Util
{
    std::vector<std::string> parse_args(int, char**);
    std::vector<std::vector<double>> normalize(std::vector<std::vector<double>>);
    std::vector<std::vector<double>> merge(std::vector<std::vector<double>>, std::vector<std::vector<double>>, double);
}

#endif