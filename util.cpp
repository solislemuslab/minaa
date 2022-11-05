#include <algorithm>
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
        std::vector<std::string> args = {"-1", "", "", "", "1", "1", "1"};

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

    /*
     * Map the labels of the will-be collapsed matrix to the indices of it.
     */
    std::vector<std::string> collapse_labels(std::vector<std::vector<double>> alignment,
        std::vector<std::string> g_labels, std::vector<std::string> h_labels)
    {
        std::vector<std::string> gh_labels;
        bool aligned = false;

        // For each gi in G, label index i in GH with gi, or gi + hj if gi is aligned with hj
        for (unsigned i = 0; i < alignment.size(); ++i)
        {
            aligned = false;
            for (unsigned j = 0; j < alignment[0].size(); ++j)
            {
                if (alignment[i][j] > 0)
                {
                    gh_labels.push_back(g_labels[i] + h_labels[j]);
                    //std::cout << g_labels[i] + h_labels[j] << std::endl; // DEBUG
                    aligned = true;
                    break;
                }
            }
            if (!aligned)
            {
                gh_labels.push_back(g_labels[i]);
                //std::cout << g_labels[i] << std::endl; // DEBUG
            }
        }

        // For each hj in H that isn't aligned with gi, label the next index with hj
        for (unsigned j = 0; j < alignment[0].size(); ++j)
        {
            aligned = false;
            for (unsigned i = 0; i < alignment.size(); ++i)
            {
                if (alignment[i][j] > 0)
                {
                    aligned = true;
                    break;
                }
            }
            if (!aligned)
            {
                gh_labels.push_back(h_labels[j]);
                //std::cout << h_labels[j] << std::endl; // DEBUG
            }
        }

        return gh_labels;
    }

    /*
     * yes
     */
    std::vector<std::vector<double>> assign(
        std::vector<std::vector<double>> collapsed,
        std::vector<std::string> gh_labels,
        std::string label1,
        std::string label2,
        unsigned value
        )
    {
        int i = std::distance(std::begin(gh_labels), std::find(std::begin(gh_labels), std::end(gh_labels), label1));
        int j = std::distance(std::begin(gh_labels), std::find(std::begin(gh_labels), std::end(gh_labels), label2));
        // If the label is not found, do something

        collapsed[i][j] = value;
        collapsed[j][i] = value;

        return collapsed;
    }


    /*
     * Collapse graph H onto graph G, with respect to the alignment.
     */
    std::vector<std::vector<double>> collapse(
        std::vector<std::vector<unsigned>> g_graph,
        std::vector<std::vector<unsigned>> h_graph,
        std::vector<std::vector<double>> alignment,
        std::vector<std::string> g_labels,
        std::vector<std::string> h_labels,
        std::vector<std::string> gh_labels)
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

        // Initialize collapsed with 0s
        std::vector<std::vector<double>> collapsed(gh_labels.size(), std::vector<double>(gh_labels.size(), 0));

        // Iterate through all nodes gi in G, aligned and unaligned
        for (unsigned gi = 0; gi < g_graph.size(); ++gi)
        {   
            unsigned hj = 0;
            for (; hj < alignment[0].size(); ++hj)
            {
                if (alignment[gi][hj] > 0) // gi and hj are aligned
                {
                    // Iterate through all nodes gk adjacent to gi
                    for (unsigned gk = 0; gk < g_graph[0].size(); ++gk)
                    {
                        if (g_graph[gi][gk] > 0) // gi and gk are adjacent
                        {
                            unsigned hl = 0;
                            for (; hl < alignment[0].size(); ++hl)
                            {
                                if (alignment[gk][hl] > 0)
                                {
                                    break; // this gk is aligned with only this hl
                                }
                            }
                            if (hl < alignment[0].size()) // gk and hl are aligned
                            {
                                auto label1 = g_labels[gi] + h_labels[hj];
                                auto label2 = g_labels[gk] + h_labels[hl];
                                if (h_graph[hj][hl] > 0) // hj is adjacent to hl
                                {
                                    collapsed = assign(collapsed, gh_labels, label1, label2, 3);
                                }
                                else // hj is not adjacent to hl
                                {
                                    collapsed = assign(collapsed, gh_labels, label1, label2, 1);
                                }
                            }
                            else // gk is unaligned
                            {
                                auto label1 = g_labels[gi] + h_labels[hj];
                                auto label2 = g_labels[gk];
                                collapsed = assign(collapsed, gh_labels, label1, label2, 1);
                            }
                        }
                    }
                    // Iterate through all nodes hk adjacent to hj
                    for (unsigned hk = 0; hk < h_graph[0].size(); ++hk)
                    {
                        if (h_graph[hj][hk] > 0) // hj and hk are adjacent
                        {
                            unsigned gl = 0;
                            //std::cout << "size(): " << alignment.size() << " " << alignment[0].size() << " hk: " << hk << std::endl; // DEBUG
                            for (; gl < alignment.size(); ++gl)
                            {
                                if (alignment[gl][hk] > 0)
                                {
                                    break; // this gl is aligned with only this hk
                                }
                            }
                            if (gl < alignment.size()) // hk and gl are aligned
                            {
                                auto label1 = g_labels[gi] + h_labels[hj];
                                auto label2 = g_labels[gl] + h_labels[hk];
                                if (g_graph[gi][gl] > 0) // gi is adjacent to gl 
                                {
                                    // collapsed = assign(collapsed, gh_labels, label1, label2, 3);
                                    continue; // we already recorded this collapse
                                }
                                else // gi is not adjacent to gl
                                {
                                    collapsed = assign(collapsed, gh_labels, label1, label2, 2);
                                }
                            }
                            else // hk is unaligned
                            {
                                auto label1 = g_labels[gi] + h_labels[hj];
                                auto label2 = h_labels[hk];
                                collapsed = assign(collapsed, gh_labels, label1, label2, 2);
                            }
                        }
                    }
                    break; // this gi is aligned with only this hj
                }
            }
            if (hj == alignment[0].size()) // gi was aligned to no hj
            {
                // Iterate through all nodes gj adjacent to gi
                for (unsigned gj = 0; gj < g_graph[0].size(); ++gj)
                {
                    if (g_graph[gi][gj] > 0) // gi and gj are adjacent
                    {
                        unsigned hk = 0;
                        for (; hk < alignment[0].size(); ++hk)
                        {
                            if (alignment[gj][hk] > 0)
                            {
                                break; // this gj is aligned with only this hk
                            }
                        }
                        if (hk < alignment[0].size()) // gj and hk are aligned
                        {
                            auto label1 = g_labels[gi];
                            auto label2 = g_labels[gj] + h_labels[hk];
                            collapsed = assign(collapsed, gh_labels, label1, label2, 1);
                        }
                        else // gj is unaligned
                        {
                            auto label1 = g_labels[gi];
                            auto label2 = g_labels[gj];
                            collapsed = assign(collapsed, gh_labels, label1, label2, 1);
                        }
                    }
                }
            }
        }

        // Iterate through the nodes hi in H that are not aligned to any node in G
        for (unsigned hi = 0; hi < h_graph.size(); ++hi)
        {   
            unsigned gj = 0;
            for (; gj < alignment.size(); ++gj)
            {
                if (alignment[gj][hi] > 0) // gj and hi are aligned
                {
                    break; // skip any aligned hi
                }
            }
            if (gj == alignment.size()) // hi was not aligned to any gj
            {
                // Iterate through all nodes hk adjacent to hi
                for (unsigned hk = 0; hk < h_graph[0].size(); ++hk)
                {
                    if (h_graph[hi][hk] > 0) // hi and hk are adjacent
                    {
                        unsigned gl = 0;
                        for (; gl < alignment.size(); ++gl)
                        {
                            if (alignment[gl][hk] > 0)
                            {
                                break; // this hk is aligned with only this gl
                            }
                        }
                        if (gl < alignment.size()) // hk and gl are aligned
                        {
                            auto label1 = h_labels[hi];
                            auto label2 = g_labels[gl] + h_labels[hk];
                            collapsed = assign(collapsed, gh_labels, label1, label2, 2);
                        }
                        else // hk is unaligned
                        {
                            auto label1 = h_labels[hi];
                            auto label2 = h_labels[hk];
                            collapsed = assign(collapsed, gh_labels, label1, label2, 2);
                        }
                    }
                }
            }
        }

        return collapsed;
    }
}