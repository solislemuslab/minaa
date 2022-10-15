#include <iostream>
#include <limits>
#include <vector>

namespace Util
{
    /*
     * Parse command line arguments.
     * args[0]: error code
     * args[1]: graph G file
     * args[2]: graph H file
     * args[3]: GDV - edge weight balancer
     * args[4]: biological data file
     * args[5]: topological - biological balancer
     */
    std::vector<std::string> parse_args(int argc, char* argv[])
    {
        std::vector<std::string> args = {"-1", "", "", "0.5", "", "0.5"};

        if (argc < 3 || argc > 5)
        {
            return args;
        }

        args[1] = argv[1];
        args[2] = argv[2];

        for (auto i = 3; i < argc; ++i)
        {
            std::string arg = std::string(argv[i]);
            if (arg.find("-a=") != std::string::npos)
            {
                args[3] = arg.substr(3);
            }
            else if (arg.find("-B=") != std::string::npos)
            {
                args[4] = arg.substr(3);
            }
            else if (arg.find("-b=") != std::string::npos)
            {
                args[5] = arg.substr(3);
            }
        }

        args[0] = "0"; // No errors
        return args;
    }

    /*
     * Normalize a matrix.
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

        // Make nonnegative
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

        // Normalize
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

    /*
     * Merge the topological and biological cost matrices.
     * beta of the weight goes to topological similarity, (1 - beta) goes to biological similarity.
     */
    std::vector<std::vector<double>> merge(
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