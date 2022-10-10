#include <vector>
#include <iostream>

namespace Util
{
    /*
     * Merge the topological and biological cost matrices.
     * alpha of the weight goes to topological similarity, (1 - alpha) goes to biological similarity.
     */
    std::vector<std::vector<double>> merge(
        std::vector<std::vector<double>> topological_costs, std::vector<std::vector<double>> biological_costs, double alpha) {
        // Handle absent biological costs
        if (biological_costs.empty()) {
            return topological_costs;
        }
        // Handle invalid alpha
        if (alpha < 0 || alpha > 1) {
            std::cerr << "Alpha must be between 0 and 1. Defaulting to alpha = 1." << std::endl;
            return topological_costs;
        }
        
        std::vector<std::vector<double>> overall_costs;

        for (unsigned i = 0; i < topological_costs.size(); ++i)
        {
            std::vector<double> row;
            for (unsigned j = 0; j < topological_costs[i].size(); ++j)
            {
                row.push_back(alpha * topological_costs[i][j] + (1 - alpha) * biological_costs[i][j]);
            }
            overall_costs.push_back(row);
        }

        return overall_costs;
    }
}