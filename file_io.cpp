// file_io.cpp
// File IO Utilities
// Reed Nelson

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <sys/stat.h>

namespace FileIO
{
    /*
     * Returns the truncated name of the file.
     */
    std::string name_file(std::string file)
    {
        auto si = file.find_last_of("/") + 1;
        auto ei = file.find_last_of(".");
        auto file_name = file.substr(si, ei - si);
        return file_name;
    }

    /*
     * Returns the name of the folder to store the output files in.
     */
    std::string name_folder(std::string g_name, std::string h_name, std::string datetime)
    {
        const std::string OUTPUT_FOLDER = "outputs/";

        // Make directory "outputs/" if it doesn't exist
        if (mkdir(OUTPUT_FOLDER.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
        {
            if(errno != EEXIST)
            {
                std::cout << "Error: could not create output folder." << std::endl;
                return "";
            }
        }
        
        // Make directory "outputs/g_name-h_name/ if it doesn't exist
        auto folder = OUTPUT_FOLDER + g_name + "-" + h_name + "_" + datetime + "/";
        if (mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
        {
            if(errno != EEXIST)
            {
                std::cout << "Error: could not create output folder." << std::endl;
                return "";
            }
        }

        // Create log file
        auto log_file = folder + "log.txt";
        std::ofstream log(log_file);
        log.close();

        return folder;
    }

    /*
     * Outputs the fiven string to the given file and std::cout.
     */
    void out(std::string file, std::string str)
    {
        std::cout << str;
        std::ofstream out(file, std::ios_base::app);
        out << str;
        out.close();
    }

    /*
     * Detect the delimiter used for a CSV file.
     */
    char detect_delimiter(std::string file)
    {
        // Open the given file, read the first line
        std::ifstream infile(file);
        std::string line;
        std::getline(infile, line);

        char delims[] = {',', ';', ' ', '\t'};
        unsigned counts[] = {0, 0, 0, 0};
        unsigned delims_length = (sizeof(delims) / sizeof(char));

        for (unsigned c = 0; c < line.length() - 1; ++c)
        {
            // Skip through all values inside quotes
            if (line[c] == '"')
            {
                ++c;
                while (line[c] != '"' && c < line.length() - 1)
                {
                    ++c;
                }
            }
            // See if this value outside quotes is a possible delimiter
            for (unsigned d = 0; d < delims_length; ++d)
            {
                if (line[c] == delims[d])
                {
                    ++counts[d];
                }
            }
        }

        unsigned most_frequent = std::distance(counts, std::max_element(counts, counts + delims_length));
        return delims[most_frequent];
    }

    /* FILE INPUT */

    /*
     * Parse the given csv adjacency matrix file into a list
     * FOR ORCA
     * TEMPORARY
     */
    std::string orca_in(std::string file)
    {
        // Identify the delimiter for this csv
        char delim = detect_delimiter(file);

        // Parse the file into a matrix
        std::ifstream infile(file);
        std::vector<std::vector<double>> matrix;
        std::string line;
        while (std::getline(infile, line))
        {
            std::stringstream ss(line);
            std::string cell;
            std::vector<double> row;
            while (std::getline(ss, cell, delim))
            {
                try
                {
                    row.push_back(std::stod(cell));
                }
                catch (std::invalid_argument const&)
                {
                    row.push_back(0);
                }
            }
            matrix.push_back(row);
        }

        // Convert the matrix into a list, with first pair being (# nodes, # edges)
        std::vector<std::array<unsigned, 2>> list;
        list.push_back({0, 0});
        for (unsigned i = 1; i < matrix.size(); ++i)
        {
            for (unsigned j = i; j < matrix[i].size(); ++j)
            {
                if (i != j && matrix[i][j] != 0)
                {
                    list.push_back({i, j});
                }
            }
        }
        list[0] = {(unsigned)(matrix.size() - 1), (unsigned)(list.size() - 1)};

        // Write the list to a file
        std::string out_file = "testdata/orca_in/" + name_file(file) + ".txt";
        std::ofstream outfile(out_file);
        for (auto &row : list)
        {
            outfile << row[0] << " " << row[1] << std::endl;
        }
        outfile.close();

        return out_file;
    }

    /*
     * Parse the given csv adjacency matrix file into a list
     * FOR GRAPHCRUNCH
     * TEMPORARY
     */
    std::string graphcrunch_in(std::string file_in, std::string file_out)
    {
        // Identify the delimiter for this csv
        char delim = detect_delimiter(file_in);
        
        // Parse the file into a matrix
        std::ifstream infile(file_in);
        std::vector<std::vector<double>> matrix;
        std::string line;
        while (std::getline(infile, line))
        {
            std::stringstream ss(line);
            std::string cell;
            std::vector<double> row;
            while (std::getline(ss, cell, delim))
            {
                try
                {
                    row.push_back(std::stod(cell));
                }
                catch (std::invalid_argument const&)
                {
                    row.push_back(0);
                }
            }
            matrix.push_back(row);
        }

        // Convert the matrix into a list
        std::vector<std::array<unsigned, 2>> list;
        for (unsigned i = 1; i < matrix.size(); ++i)
        {
            for (unsigned j = i; j < matrix[i].size(); ++j)
            {
                if (matrix[i][j] != 0) // i != j && 
                {
                    list.push_back({i, j});
                }
            }
        }

        // Write the list to a file
        std::string out_file = file_out + ".csv";
        std::ofstream outfile(out_file);
        outfile << (matrix.size() - 1) << std::endl;
        outfile << list.size() << std::endl;
        for (auto &row : list)
        {
            outfile << row[0] << " " << row[1] << std::endl;
        }
        outfile.close();

        return out_file;
    }

    /*
     * Parse the file at the given path into a graph object
     * Require that the file is a CSV
     * Improvement?: change from unsigned to 1 byte or bool or something
     */
    std::vector<std::vector<unsigned>> file_to_graph(std::string file) 
    {
        // Identify the delimiter for this csv
        char delim = detect_delimiter(file);

        // Parse the file directly into a matrix
        std::ifstream infile(file);
        std::vector<std::vector<double>> matrix;
        std::string line;
        while (std::getline(infile, line))
        {
            std::stringstream ss(line);
            std::string cell;
            std::vector<double> row;
            while (std::getline(ss, cell, delim))
            {
                try
                {
                    row.push_back(std::stod(cell));
                }
                catch (std::invalid_argument const&)
                {
                    row.push_back(0);
                }
            }
            matrix.push_back(row);
        }

        // Convert the matrix into a graph
        std::vector<std::vector<unsigned>> graph;
        for (unsigned i = 1; i < matrix.size(); ++i)
        {
            std::vector<unsigned> row;
            for (unsigned j = 1; j < matrix[i].size(); ++j)
            {
                if (matrix[i][j] != 0)
                {
                    row.push_back(1);
                }
                else
                {
                    row.push_back(0);
                }
            }
            graph.push_back(row);
        }

        return graph;
    }

    /*
     * Parse the labels
     */
    std::vector<std::string> parse_labels(std::string file)
    {
        std::ifstream infile(file);
        std::vector<std::string> labels;
        std::string line;
        std::getline(infile, line);

        while (std::getline(infile, line))
        {
            std::stringstream ss(line);
            std::string cell;
            std::getline(ss, cell, ',');
            labels.push_back(cell);
        }
        return labels;
    }
    
    /*
     * Parse the file at the given path into a vector of integer arrays.
     */
    std::vector<std::vector<unsigned>> file_to_gdvs(std::string file)
    {
        // Identify the delimiter for this csv
        char delim = detect_delimiter(file);

        // Parse the file
        std::ifstream infile(file);
        std::vector<std::vector<unsigned>> gdvs;
        std::string line;
        int c = 0;
        std::cout << "file: " << file << std::endl;
        while (std::getline(infile, line))
        {
            c++;
            std::istringstream ss(line);
            std::string val;
            std::vector<unsigned> gdv;
            while (std::getline(ss, val, delim))
            {
                try
                {
                    gdv.push_back(std::stoi(val));
                }
                catch (std::invalid_argument const&)
                {
                    gdv.push_back(0);
                }
            }
            gdvs.push_back(gdv);

        }
        infile.close();
        std::cout << "Parsed " << c << " lines" << std::endl; // DEBUG

        return gdvs;
    }

    /*
     * Parse the file at the given path into a double vector vector.
     */
    std::vector<std::vector<double>> file_to_cost(std::string filestr)
    {
        std::ifstream file(filestr);
        std::vector<std::vector<double>> costs;
        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::vector<double> row;
            double val;
            while (iss >> val)
            {
                row.push_back(val);
            }
            costs.push_back(row);
        }
        return costs;
    }

    /* FILE OUTPUT */

    /*
     * Write GDVs to a file
     */
    std::string gdvs_to_file(std::string folder, std::string graph_name,
        std::vector<std::string> labels, std::vector<std::vector<unsigned>> gdvs)
    {
        std::string filestr = folder + graph_name + "_gdvs.csv";

        std::ofstream fout(filestr);
        if (!fout.is_open())
        {
            std::cerr << "Could not open file " << filestr << std::endl;
            return "";
        }

        for (unsigned i = 0; i < labels.size(); ++i)
        {
            fout << labels[i];
            for (unsigned j = 0; j < gdvs[i].size(); ++j)
            {
                fout << "," << gdvs[i][j];
            }
            fout << std::endl;
        }
        fout.close();

        return filestr;
    }

    /*
     * Write the cost matrix to a file.
     */
    std::string cost_to_file(std::string folder, std::vector<std::string> g_labels,
        std::vector<std::string> h_labels, std::vector<std::vector<double>> cost)
    {
        std::string filestr = "top_costs.csv";

        std::ofstream fout(folder + filestr);
        if (!fout.is_open())
        {
            std::cerr << "Could not open file " << filestr << std::endl;
            return "";
        }

        fout << "\"\"";
        for (unsigned i = 0; i < cost[0].size(); ++i)
        {
            fout << "," << h_labels[i];
        }
        for (unsigned i = 0; i < cost.size(); ++i)
        {
            fout << std::endl << g_labels[i];
            for (unsigned j = 0; j < cost[i].size(); ++j)
            {
                fout << "," << cost[i][j];
            }
        }
        fout.close();

        return filestr;
    }

    /*
     * Write the given alignment to a csv file as a matrix.
     */
    std::string alignment_to_matrix_file(std::string folder, std::vector<std::string> g_labels,
        std::vector<std::string> h_labels, std::vector<std::vector<double>> alignment, double gamma)
    {
        std::string path = folder + "alignment_matrix.csv";
        std::ofstream fout(path);

        fout << "\"\"";
        for (unsigned i = 0; i < alignment[0].size(); ++i)
        {
            fout << "," << h_labels[i];
        }
        for (unsigned i = 0; i < alignment.size(); ++i)
        {
            fout << std::endl << g_labels[i];
            for (unsigned j = 0; j < alignment[0].size(); ++j)
            {
                if (alignment[i][j] < gamma)
                {
                    fout << "," << alignment[i][j];
                }
                else
                {
                    fout << ",0";
                }
            }
        }
        fout.close();

        return path;
    }

    /*
     * Write the given alignment to a csv file as a list.
     */
    std::string alignment_to_list_file(std::string folder, std::vector<std::string> g_labels,
        std::vector<std::string> h_labels, std::vector<std::vector<double>> alignment, double gamma)
    {
        // Convert the alignment matrix into a list
        std::vector<std::array<double, 3>> list;
        double net_cost = 0;
        for (unsigned i = 0; i < alignment.size(); ++i)
        {
            for (unsigned j = 0; j < alignment[0].size(); ++j)
            {
                if (alignment[i][j] > 0 && alignment[i][j] < gamma)
                {
                    net_cost += alignment[i][j];
                    list.push_back({(double)i, (double)j, alignment[i][j]});
                }
            }
        }

        // Write the list to a file
        std::string path = folder + "alignment_list.csv";
        std::ofstream fout(path);
        fout << net_cost << "," << "" << "," << "" << std::endl; // DO SOMETHING?
        for (unsigned i = 0; i < list.size(); ++i)
        {
            fout << g_labels[list[i][0]] << "," << h_labels[list[i][1]] << "," << list[i][2] << std::endl;
        }
        fout.close();

        return path;
    }

    /*
     * Write the bridge graph to a file.
     */
    std::string bridge_to_file(std::string folder, std::vector<std::string> g_labels,
        std::vector<std::string> h_labels, std::vector<std::vector<double>> bridge_graph)
    {
        std::string filestr = "bridge.csv";

        std::ofstream fout(folder + filestr);
        if (!fout.is_open())
        {
            std::cerr << "Could not open file " << filestr << std::endl;
            return "";
        }

        // Write the first row
        fout << "\"\"";
        for (unsigned i = 0; i < g_labels.size(); ++i)
        {
            fout << "," << g_labels[i];
        }
        for (unsigned i = 0; i < h_labels.size(); ++i)
        {
            fout << "," << h_labels[i];
        }

        // Write the rows with G labels
        for (unsigned i = 0; i < g_labels.size(); ++i)
        {
            fout << std::endl << g_labels[i];
            for (unsigned j = 0; j < bridge_graph.size(); ++j)
            {
                fout << "," << bridge_graph[i][j];
            }
        }

        // Write the rows with H labels
        for (unsigned i = 0; i < h_labels.size(); ++i)
        {
            fout << std::endl << h_labels[i];
            for (unsigned j = 0; j < bridge_graph.size(); ++j)
            {
                fout << "," << bridge_graph[g_labels.size() + i][j];
            }
        }
        
        fout.close();

        return filestr;
    }

    /*
     * Write the collapsed matrix to a file.
     */
    std::string collapse_to_file(std::string folder, std::vector<std::string> gh_labels,
        std::vector<std::vector<double>> collapsed)
    {
        std::string filestr = "collapsed.csv";

        std::ofstream fout(folder + filestr);
        if (!fout.is_open())
        {
            std::cerr << "Could not open file " << filestr << std::endl;
            return "";
        }

        fout << "\"\"";
        for (unsigned i = 0; i < collapsed[0].size(); ++i)
        {
            fout << "," << gh_labels[i];
        }
        for (unsigned i = 0; i < collapsed.size(); ++i)
        {
            fout << std::endl << gh_labels[i];
            for (unsigned j = 0; j < collapsed[i].size(); ++j)
            {
                fout << "," << collapsed[i][j];
            }
        }
        fout.close();

        return filestr;
    }

}