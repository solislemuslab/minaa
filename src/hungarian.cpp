// align.cpp
// Hungarian Algorithm O(n^3)
// Reed Nelson

/*
 * BUG:
 * The algorithm occasionally breaks on inputs where graph G > graph H.
 */

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <vector>

namespace Hungarian
{
    int prog = 0;                                               // PROGRESS
    void print_progress(double val)                             // PROGRESS
    {                                                           // PROGRESS
        std::cout << "\33[2K\r~ " << (val / 0.15) * 100 << "%"; // PROGRESS
    }                                                           // PROGRESS

    const int MAX = 1;

    /* 
     * If the matrix is not square, pad it so it is.
     */
    void pad_matrix(std::vector<std::vector<double>> &matrix)
    {
        unsigned i_size = matrix.size();
        unsigned j_size = matrix[0].size();

        if (i_size > j_size)
        {
            for (auto &vec : matrix)
            {
                vec.resize(i_size, MAX);
            }   
        }
        else if (i_size < j_size)
        {
            while (matrix.size() < j_size)
            {
                matrix.push_back(std::vector<double>(j_size, MAX));
            } 
        }
    }

    /* 
     * Clear the temporary vectors.
     */
    inline void clear_covers(std::vector<unsigned char> &cover)
    {
        for (auto &n : cover)
        {
            n = 0;
        }
    }

    /* 
     * Wipe all primes from the mask matrix.
     */
    void clear_primes(std::vector<std::vector<unsigned char>> &mask)
    {
        for (auto &row : mask)
        {
            for (auto &val : row)
            {
                if (val == 2)
                {
                    val = 0;
                }
            }
        }      
    }

    /* 
     * 
     */
    void find_a_zero(int &row, int &col, const std::vector<std::vector<double>> &costs,
                     const std::vector<unsigned char> &row_cover, const std::vector<unsigned char> &col_cover)
    {
        unsigned r = 0;
        unsigned c = 0;
        bool done = false;
        row = -1;
        col = -1;

        while (!done)
        {
            c = 0;
            while (true)
            {
                if (costs[r][c] == 0 && row_cover[r] == 0 && col_cover[c] == 0)
                {
                    row = r;
                    col = c;
                    done = true;
                }
                ++c;
                if (c >= costs.size() || done)
                {
                    break;
                }
            }
            ++r;
            if (r >= costs.size())
            {
                done = true;
            }        
        }
    }

    /* 
     * Returns true if there is a starred zero in the given row.
     */
    bool star_in_row(int row, const std::vector<std::vector<unsigned char>> &mask)
    {
        for (unsigned c = 0; c < mask.size(); ++c)
        {
            if (mask[row][c] == 1)
            {
                return true;
            }
        }
        return false;
    }

    /* 
     * Sets col as the column containing the first starred zero in the given row, if there is one.
     */
    void find_star_in_row(int row, int &col, const std::vector<std::vector<unsigned char>> &mask)
    {
        col = -1;
        for (unsigned c = 0; c < mask.size(); ++c)
        {
            if (mask[row][c] == 1)
            {
                col = c;
                return;
            }
        }   
    }

    /* 
     * Sets row as the row containing the first starred zero in the given column, if there is one.
     */
    void find_star_in_col(int col, int &row, const std::vector<std::vector<unsigned char>> &mask)
    {
        row = -1;
        for (unsigned r = 0; r < mask.size(); ++r)
        {
            if (mask[r][col] == 1)
            {
                row = r;
                return;
            }
        }
    }

    /* 
     * Sets col as the column containing the first primed zero in the given row, if there is one.
     */
    void find_prime_in_row(int row, int &col, const std::vector<std::vector<unsigned char>> &mask)
    {
        for (unsigned c = 0; c < mask.size(); ++c)
        {
            if (mask[row][c] == 2)
            {
                col = c;
                return;
            }
        }   
    }

    /* 
     * 
     */
    void augment_path(std::vector<std::vector<int>> &path, unsigned path_count, std::vector<std::vector<unsigned char>> &mask)
    {
        for (unsigned p = 0; p < path_count; ++p)
        {
            if (mask[path[p][0]][path[p][1]] == 1)
            {
                mask[path[p][0]][path[p][1]] = 0;
            }
            else
            {
                mask[path[p][0]][path[p][1]] = 1;   
            }
        }
    }

    /* 
     * Find the smallest uncovered value in the cost matrix.
     */
    void find_smallest(double &minval, const std::vector<std::vector<double>> &costs,
                       const std::vector<unsigned char> &row_cover, const std::vector<unsigned char> &col_cover)
    {
        for (unsigned r = 0; r < costs.size(); ++r)
        {
            for (unsigned c = 0; c < costs.size(); ++c)
            {
                if (row_cover[r] == 0 && col_cover[c] == 0)
                {
                    if (minval > costs[r][c])
                    {
                        minval = costs[r][c];
                    }
                }
            }
        }       
    }

    /* END OF UTILITY FUNCTIONS */

    /* 
     * Reduce each row/col subtracting the minimum value in each row/col from all elements it.
     */
    void step1(std::vector<std::vector<double>> &costs, int &step)
    {
        // For each row of the matrix, find the smallest element and subtract it from every element in its row.
        for (auto &row : costs)
        {
            auto smallest = *std::min_element(begin(row), end(row));
            if (smallest > 0)
            {
                for (auto &n : row)
                {
                    n -= smallest;
                }
            }  
        }

        // For each col of the matrix, find the smallest element and subtract it from every element in its col. 
        for (unsigned j = 0; j < costs.size(); ++j)
        {
            double minval = MAX;
            for (unsigned i = 0; i < costs.size(); ++i)
            {
                minval = std::min(minval, costs[i][j]);
            }

            if (minval > 0)
            {
                for (unsigned i = 0; i < costs.size(); ++i)
                {
                    costs[i][j] -= minval;
                }
            }
        }

        step = 2;
    }

    /* Find a zero (Z) in the resulting matrix.  If there is no starred zero in its row or
     * column, star Z. Repeat for each element in the matrix. Go to Step 3.  In this step,
     * we introduce the mask matrix which in the same dimensions as the cost matrix and
     * is used to star and prime zeros of the cost matrix.  If mask(i,j)=1 then C(i,j) is a
     * starred zero,  If mask(i,j)=2 then C(i,j) is a primed zero.  We also define two vectors
     * row_cover and col_cover that are used to "cover" the rows and columns of the cost matrix.
     * In the nested loop (over indices i and j) we check to see if C(i,j) is a zero value
     * and if its column or row is not already covered.  If not then we star this zero
     * (i.e. set mask(i,j)=1) and cover its row and column (i.e. set R_cov(i)=1 and C_cov(j)=1).
     * Before we go on to Step 3, we uncover all rows and columns so that we can use the
     * cover vectors to help us count the number of starred zeros.
     */
    void step2(const std::vector<std::vector<double>> &costs, std::vector<std::vector<unsigned char>> &mask,
               std::vector<unsigned char> &row_cover, std::vector<unsigned char> &col_cover, int &step)
    {
        for (unsigned r = 0; r < costs.size(); ++r)
        {
            for (unsigned c = 0; c < costs.size(); ++c)
            {
                if (costs[r][c] == 0)
                {
                    if (row_cover[r] == 0 && col_cover[c] == 0)
                    {
                        mask[r][c] = 1;
                        row_cover[r] = 1;
                        col_cover[c] = 1;
                    }
                }
            }
        }

        clear_covers(row_cover); // reset vectors for posterior using
        clear_covers(col_cover);

        step = 3;
    }

    /*
     * Cover each column containing a starred zero.  If all columns are covered, we are done. Else, go to Step 4.
     */
    void step3(const std::vector<std::vector<unsigned char>> &mask, std::vector<unsigned char> &col_cover, int &step)
    {
        unsigned colcount = 0;

        // Cover each column containing a starred zero.  
        for (unsigned r = 0; r < mask.size(); ++r)
        {
            for (unsigned c = 0; c < mask.size(); ++c)
            {
                if (mask[r][c] == 1)
                {
                    col_cover[c] = 1;
                }
            }
        }
            
        for (auto &n : col_cover) {
            if (n == 1) {
                ++colcount;
            }
        }
        
        // If all columns are covered, the starred zeros make complete set of unique assignments.
        if (colcount >= mask.size())
        {
            step = 7; // solution found
        }
        else
        {
            step = 4;
        }
    }

    /* 
     * Find a noncovered zero and prime it. Save the smallest uncovered value and Go to Step 6.
     */
    void step4(const std::vector<std::vector<double>> &costs, std::vector<std::vector<unsigned char>> &mask,
               std::vector<unsigned char> &row_cover, std::vector<unsigned char> &col_cover,
               int &path_row_0, int &path_col_0, int &step)
    {
        int row = -1;
        int col = -1;
        bool done = false;

        while (!done) // while there are uncovered zeros
        {
            find_a_zero(row, col, costs, row_cover, col_cover);

            if (row == -1)
            {
                done = true;
                step = 6;
            }
            else
            {
                double p = double(prog++) / (costs.size() * costs.size()); // PROGRESS
                print_progress(p);                                         // PROGRESS
                
                mask[row][col] = 2;
                if (star_in_row(row, mask)) 
                {
                    // cover this row and uncover the column containing the starred zero
                    find_star_in_row(row, col, mask);
                    row_cover[row] = 1;
                    col_cover[col] = 0;
                }
                else // there is no starred zero in the row containing this primed zero
                {
                    done = true;
                    step = 5;
                    path_row_0 = row;
                    path_col_0 = col;
                }
            }
        }
    }

    /*
     * Construct a series of alternating primed and starred zeros as follows.
     * Let Z0 represent the uncovered primed zero found in Step 4.  Let Z1 denote the
     * starred zero in the column of Z0 (if any). Let Z2 denote the primed zero in the
     * row of Z1 (there will always be one).  Continue until the series terminates at a
     * primed zero that has no starred zero in its column.  Unstar each starred zero of
     * the series, star each primed zero of the series, erase all primes and uncover every
     * line in the matrix.  Return to Step 3. Step 5 is a verbal description of the
     * augmenting path algorithm (for solving the maximal matching problem).
     */
    void step5(std::vector<std::vector<int>> &path, int path_row_0, int path_col_0, std::vector<std::vector<unsigned char>> &mask,
               std::vector<unsigned char> &row_cover, std::vector<unsigned char> &col_cover, int &step)
    {
        int r = -1;
        int c = -1;
        unsigned path_count = 1;

        path[path_count - 1][0] = path_row_0;
        path[path_count - 1][1] = path_col_0;

        bool done = false;
        while (!done)
        {
            find_star_in_col(path[path_count - 1][1], r, mask);
            if (r > -1)
            {
                ++path_count;
                path[path_count - 1][0] = r;
                path[path_count - 1][1] = path[path_count - 2][1];
            }
            else
            {
                done = true;
            }

            if (!done)
            {
                find_prime_in_row(path[path_count - 1][0], c, mask);
                ++path_count;
                path[path_count - 1][0] = path[path_count - 2][0];
                path[path_count - 1][1] = c;
            }
        }

        augment_path(path, path_count, mask);
        clear_covers(row_cover);
        clear_covers(col_cover);
        clear_primes(mask);

        step = 3;
    }

    /* 
     * Add the value found in Step 4 to every element of each covered row, and subtract it
     * from every element of each uncovered column.  Return to Step 4 without altering any
     * stars, primes, or covered lines. Notice that this step uses the smallest uncovered
     * value in the cost matrix to modify the matrix.  Even though this step refers to the
     * value being found in Step 4 it is more convenient to wait until you reach Step 6
     * before searching for this value.  It may seem that since the values in the cost
     * matrix are being altered, we would lose sight of the original problem.
     * However, we are only changing certain values that have already been tested and
     * found not to be elements of the minimal assignment.  Also we are only changing the
     * values by an amount equal to the smallest value in the cost matrix, so we will not
     * jump over the optimal (i.e. minimal assignment) with this change.
     */
    void step6(std::vector<std::vector<double>> &costs, const std::vector<unsigned char> &row_cover,
               const std::vector<unsigned char> &col_cover, int &step)
    {
        double minval = MAX;
        find_smallest(minval, costs, row_cover, col_cover);

        for (unsigned r = 0; r < costs.size(); ++r)
        {
            for (unsigned c = 0; c < costs.size(); ++c)
            {
                if (row_cover[r] == 1)
                {
                    costs[r][c] += minval;
                }  
                if (col_cover[c] == 0)
                {
                    costs[r][c] -= minval;
                }
            }
        }

        step = 4;
    }
    
    /*
     * Calculates the optimal cost from mask matrix.
     */
    std::vector<std::vector<double>> output_solution(const std::vector<std::vector<double>> &original,
        const std::vector<std::vector<unsigned char>> &mask)
    {
        std::vector<std::vector<double>> alignment;

        for (unsigned r = 0; r < original.size(); ++r)
        {
            std::vector<double> row;
            for (unsigned c = 0; c < original[r].size(); ++c)
            {
                if (mask[r][c] != 0)
                {
                    row.push_back(1 - original[r][c]);
                }
                else
                {
                    row.push_back(0);
                }
            }
            alignment.push_back(row);
        }
        return alignment;
    }

    /*
     * Driver code.
     */
    std::vector<std::vector<double>> hungarian(std::vector<std::vector<double>> original)
    {
        // Validate input values
        for (auto vec : original)
        {
            for (auto val : vec)
            {
                if (val < 0 || val > MAX)
                {
                    std::cout << "Error: Input contains invalid values. All entries must be in range [0,1]." << std::endl;
                    return {};
                }
            }
        }

        // Duplicate original matrix
        std::vector<std::vector<double>> costs(original.size(), std::vector<double>(original.begin()->size()));
        auto it = original.begin();
        for (auto &vec : costs)
        {
            std::copy(it->begin(), it->end(), vec.begin());
            it = std::next(it);
        }

        // Make the matrix square
        pad_matrix(costs);

        // mask(i,j)=1 -> C(i,j) is a starred zero, mask(i,j)=2 -> C(i,j) is a primed zero
        std::vector<std::vector<unsigned char>> mask(costs.size(), std::vector<unsigned char>(costs.size(), 0));

        /* We also define two vectors row_cover and col_cover that are used to "cover"
         * the rows and columns of the cost matrix C
         */
        std::vector<unsigned char> row_cover(costs.size(), 0);
        std::vector<unsigned char> col_cover(costs.size(), 0);

        int path_row_0; // temporary to hold the smallest uncovered value
        int path_col_0; 

        // Array for the augmenting path algorithm
        std::vector<std::vector<int>> path(costs.size() + 1, std::vector<int>(2, 0));

        bool done = false;
        int step = 1;

        while (!done)
        {
            switch (step)
            {
            case 1:
                
                step1(costs, step);
                break;
            case 2:
                step2(costs, mask, row_cover, col_cover, step);
                break;
            case 3:
                step3(mask, col_cover, step);
                break;
            case 4:
                step4(costs, mask, row_cover, col_cover, path_row_0, path_col_0, step);
                break;
            case 5:
                step5(path, path_row_0, path_col_0, mask, row_cover, col_cover, step);
                break;
            case 6:
                step6(costs, row_cover, col_cover, step);
                break;
            case 7:
                for (auto &vec : mask)
                {
                    vec.resize(original.begin()->size());
                }
                mask.resize(original.size());
                done = true;
                break;
            default:
                done = true;
                break;
            }
        }

        return output_solution(original, mask);
    }

} // end of namespace Hungarian