#include <algorithm>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <vector>

#include "file_io.h"

namespace Util
{
    /**
     * Gets the current date and time, formatted as a string.
     * 
     * @return the current date and time, formatted as a string.
     * 
     * @throws 
     */
    std::string now()
    {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y_%m_%d-%H_%M_%S");
        std::string str = oss.str();

        return str;
    }

    /**
     * Returns the given double as a string, with the given number of decimal places.
     * 
     * @param d The double to convert to a string.
     * @param n Te number of decimal places to include in the string.
     * 
     * @return The given double as a string, with the given number of decimal places.
     */
    std::string to_string(double d, int n)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(n) << d;
        return oss.str();
    }

    /**
     * Parse command line arguments.
     * args[0]:  argv[0]
     * args[1]:  graph G file
     * args[2]:  graph H file
     * args[3]:  biological data file
     * args[4]:  GDV - edge weight balancer
     * args[5]:  topological - biological balancer
     * args[6]:  G alias
     * args[7]:  H alias
     * args[8]:  B alias
     * args[9]:  do a passthrough?
     * args[10]: include a timestamp?
     * args[11]: include a greekstamp?
     * 
     * @param argc The number of command line arguments.
     * @param argv The command line arguments.
     * 
     * @return A list with values for all command line arguments, in a certain order.
     * 
     * @throws std::invalid_argument if an argument is misformatted.
     */
    std::vector<std::string> parse_args(int argc, char* argv[])
    {
        std::vector<std::string> args = {"", "", "", "", "1", "1", "", "", "", "0", "0", "0"};

        if (argc < 3 || argc > 12)
        {
            throw std::invalid_argument("Invalid number of arguments.\nUsage: ./minaaa.exe <G.csv> <H.csv> \nSee README.md for additional options and details.");
        }

        if (!FileIO::is_accessible(argv[1]))
        {
            throw std::invalid_argument("The first argument specified must be a readable file.");
        }
        if (!FileIO::is_accessible(argv[2]))
        {
            throw std::invalid_argument("The second argument specified must be a readable file.");
        }

        args[0] = argv[0];
        args[1] = argv[1];
        args[2] = argv[2];

        for (auto i = 3; i < argc; ++i)
        {
            std::string arg = std::string(argv[i]);
            if (arg.find("-B=") != std::string::npos)
            {
                args[3] = arg.substr(3);
                if (!FileIO::is_accessible(args[3]))
                {
                    throw std::invalid_argument("The biological data file cannot be read.");
                }
            }
            else if (arg.find("-a=") != std::string::npos)
            {
                args[4] = arg.substr(3);
                if (std::stod(args[4]) < 0 || std::stod(args[4]) > 1)
                {
                    throw std::invalid_argument("The alpha argument must be in range [0, 1].");
                }
            }
            else if (arg.find("-b=") != std::string::npos)
            {
                args[5] = arg.substr(3);
                if (std::stod(args[5]) < 0 || std::stod(args[5]) > 1)
                {
                    throw std::invalid_argument("The beta argument must be in range [0, 1].");
                }
            }
            else if (arg.find("-Galias=") != std::string::npos)
            {
                args[6] = arg.substr(8);
                if (!FileIO::is_valid_filename(args[6]))
                {
                    throw std::invalid_argument("The G alias contains an illegal character.");
                }
            }
            else if (arg.find("-Halias=") != std::string::npos)
            {
                args[7] = arg.substr(8);
                if (!FileIO::is_valid_filename(args[7]))
                {
                    throw std::invalid_argument("The H alias contains an illegal character.");
                }
            }
            else if (arg.find("-Balias=") != std::string::npos)
            {
                args[8] = arg.substr(8);
                if (!FileIO::is_valid_filename(args[8]))
                {
                    throw std::invalid_argument("The B alias contains an illegal character.");
                }
            }
            else if (arg.find("-p") != std::string::npos)
            {
                args[9] = "1";
            }
            else if (arg.find("-t") != std::string::npos)
            {
                args[10] = "1";
            }
            else if (arg.find("-g") != std::string::npos)
            {
                args[11] = "1";
            }
            else
            {
                throw std::invalid_argument("Invalid argument: " + arg);
            }
        }

        return args;
    }

    /**
     * Parse the given double matrix into a binary matrix.
     * 
     * @param double_matrix The matrix to convert to binary.
     * 
     * @return A binary matrix.
     */
    std::vector<std::vector<unsigned>> binarify(std::vector<std::vector<double>> double_matrix)
    {
        std::vector<std::vector<unsigned>> binary_matrix;
        for (unsigned i = 0; i < double_matrix.size(); ++i)
        {
            std::vector<unsigned> row;
            for (unsigned j = 0; j < double_matrix[i].size(); ++j)
            {
                if (double_matrix[i][j] != 0)
                {
                    row.push_back(1);
                }
                else
                {
                    row.push_back(0);
                }
            }
            binary_matrix.push_back(row);
        }

        return binary_matrix;
    }

    /**
     * Normalize the entries of the given matrix to be in range [0, 1].
     * 
     * @param matrix The matrix to normalize.
     * 
     * @return The normalized matrix.
     * 
     * @throws 
     */
    std::vector<std::vector<double>> normalize(std::vector<std::vector<double>> matrix)
    {
        // Find the max and min values in the matrix
        double min = std::numeric_limits<double>::max();
        double max = std::numeric_limits<double>::min();
        for (unsigned i = 0; i < matrix.size(); ++i)
        {
            std::vector<double> row;
            for (unsigned j = 0; j < matrix[i].size(); ++j)
            {
                if (matrix[i][j] < min)
                {
                    min = matrix[i][j];
                }
                if (matrix[i][j] > max)
                {
                    max = matrix[i][j];
                }
            }
        }

        // Make all values non-negative
        if (min < 0)
        {
            // Shift all values up by the min value
            for (unsigned i = 0; i < matrix.size(); ++i)
            {
                for (unsigned j = 0; j < matrix[i].size(); ++j)
                {
                    matrix[i][j] += std::abs(min);
                }
            }
            max += std::abs(min);
            min = 0;
        }

        // Rescale such that max is 1
        std::vector<std::vector<double>> norm_matrix;
        for (unsigned i = 0; i < matrix.size(); ++i)
        {
            std::vector<double> row;
            for (unsigned j = 0; j < matrix[i].size(); ++j)
            {
                row.push_back(matrix[i][j] / max);
            }
            norm_matrix.push_back(row);
        }

        return norm_matrix;
    }

    /**
     * Combine the topological and biological cost matrices.
     * 
     * @param topological_costs The topological cost matrix.
     * @param biological_costs The biological cost matrix.
     * @param beta of the weight goes to topological similarity, (1 - beta) goes to biological similarity.
     * 
     * @return The combined cost matrix.
     * 
     * @throws 
     */
    std::vector<std::vector<double>> combine(
        std::vector<std::vector<double>> topological_costs, std::vector<std::vector<double>> biological_costs, double beta) {
        // Handle absent biological costs
        if (biological_costs.empty()) {
            return topological_costs;
        }
        // Handle invalid beta
        if (beta < 0 || beta > 1) {
            std::cerr << "Beta must be between 0 and 1. Defaulting to beta = 1." << std::endl;
            return topological_costs;
        }
        
        std::vector<std::vector<double>> overall_costs;

        for (unsigned i = 0; i < topological_costs.size(); ++i)
        {
            std::vector<double> row;
            for (unsigned j = 0; j < topological_costs[i].size(); ++j)
            {
                row.push_back(beta * topological_costs[i][j] + (1 - beta) * biological_costs[i][j]);
            }
            overall_costs.push_back(row);
        }

        return overall_costs;
    }
    
}
