#include <ctime>
#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>

namespace Util
{
    /*
     * Gets the current date and time, formatted as a string.
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

    /*
     * Parse command line arguments.
     * args[0]: error code
     * args[1]: graph G file
     * args[2]: graph H file
     * args[3]: biological data file
     * args[4]: GDV - edge weight balancer
     * args[5]: topological - biological balancer
     * args[6]: alignment cost threshold
     */
    std::vector<std::string> parse_args(int argc, char* argv[])
    {
        std::vector<std::string> args = {"-1", "", "", "", "0.5", "0.5", "1"};

        if (argc < 3 || argc > 7)
        {
            return args;
        }

        args[1] = argv[1];
        args[2] = argv[2];

        for (auto i = 3; i < argc; ++i)
        {
            std::string arg = std::string(argv[i]);
            if (arg.find("-B=") != std::string::npos)
            {
                args[3] = arg.substr(3);
            }
            else if (arg.find("-a=") != std::string::npos)
            {
                args[4] = arg.substr(3);
            }
            else if (arg.find("-b=") != std::string::npos)
            {
                args[5] = arg.substr(3);
            }
            else if (arg.find("-g=") != std::string::npos)
            {
                args[6] = arg.substr(3);
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

    /*
     * Collapse graph H onto graph G, with respect to the alignment.
     */
    std::vector<std::vector<double>> collapse(std::vector<std::vector<unsigned>> g_graph,
        std::vector<std::vector<unsigned>> h_graph, std::vector<std::vector<double>> alignment)
    {
        // Denote the union (wrt the alignment) of the nodes in G and H as GH
        // GOAL: an adj matrix GH x GH in which...
        // - GH_i,j = 0 if there is no edge between nodes i and j
        // - GH_i,j = 1 if only G draws an edge between nodes i and j
        // - GH_i,j = 2 if only H draws an edge between nodes i and j
        // - GH_i,j = 3 if both G and H draw an edge between nodes i and j

        // Layout
        // Break GH into 2 disjoint sets: G and H - alignment(G, H), and operate separately
        // For each possible edge in the G adjacency matrix, 

        // Get |GH|
        unsigned gh_size = g_graph.size() + h_graph.size() - alignment.size();
        // Initialize collapsed with 0s
        std::vector<std::vector<double>> collapsed(gh_size, std::vector<double>(gh_size, 0));

        // Iterate through the possible edges in G
        for (unsigned i = 0; i < g_graph.size(); ++i)
        {
            for (unsigned j = 0; j < g_graph.size(); ++j)
            {
                // Check if G has an edge here
                if (g_graph[i][j] > 0)
                {
                    collapsed[i][j] += 1;
                }

                // Check if H has an edge here
                // Check if G_i corresponds to some H_k
                unsigned k;
                for (k = 0; k < alignment[i].size(); ++k)
                {
                    if (alignment[i][k] > 0)
                    {
                        break;
                    }
                }
                
                if (k < alignment[i].size()) // if G_i is aligned to H_k...
                {
                    // Check if H_k shares an edge with an H_l who is aligned to G_j
                    for (unsigned l = 0; l < h_graph[k].size(); ++l)
                    {
                        if (h_graph[k][l] > 0)
                        {
                            if (alignment[j][l] > 0)
                            {
                                collapsed[i][j] += 2;
                            }
                        }
                    }
                }
            }
        }

        // Identify the nodes only in H
        std::vector<unsigned> h_only;
        for (unsigned i = 0; i < h_graph.size(); ++i)
        {
            bool is_aligned = false;
            for (unsigned j = 0; j < h_graph.size(); ++j)
            {
                // Check if H has an edge here
                if (h_graph[i][j] > 0)
                {
                    is_aligned = true;
                    break;
                }
            }
            if (!is_aligned)
            {
                h_only.push_back(i);
            }
        }

        // Iterate through the nodes only in H

        return collapsed;
    }

    /*
     * Bridge graphs G and H, with respect to the alignment.
     * Returns simply:
     * [    G    ][A(G, H)]
     * [A(G, H)^T][   H   ]
     */
    std::vector<std::vector<double>> bridge(std::vector<std::vector<unsigned>> g_graph,
        std::vector<std::vector<unsigned>> h_graph, std::vector<std::vector<double>> alignment)
    {
        // Initialize bridged with 0s
        std::vector<std::vector<double>> bridged(g_graph.size() + h_graph.size(), std::vector<double>(g_graph.size() + h_graph.size(), 0));

        // Iterate through the first G rows
        for (unsigned i = 0; i < g_graph.size(); ++i)
        {
            // Iterate through the first G columns
            for (unsigned j = 0; j < g_graph.size(); ++j)
            {
                if (g_graph[i][j] > 0)
                {
                    bridged[i][j] = 1;
                }
            }
            // Iterate through the last H columns
            for (unsigned j = 0; j < h_graph.size(); ++j)
            {
                if (alignment[i][j] > 0)
                {
                    bridged[i][g_graph.size() + j] = 1;
                }
            }
        }
        
        // Iterate through the last H rows
        for (unsigned i = 0; i < h_graph.size(); ++i)
        {
            // Iterate through the first G columns
            for (unsigned j = 0; j < g_graph.size(); ++j)
            {
                if (alignment[j][i] > 0)
                {
                    bridged[g_graph.size() + i][j] = 1;
                }
            }
            // Iterate through the last H columns
            for (unsigned j = 0; j < h_graph.size(); ++j)
            {
                if (h_graph[i][j] > 0)
                {
                    bridged[g_graph.size() + i][g_graph.size() + j] = 1;
                }
            }
        }

        return bridged;
    }
}