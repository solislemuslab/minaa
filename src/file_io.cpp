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
    /**
     * Returns whether or not the given string is to a readable file.
     * 
     * @param file The file to check.
     * 
     * @return True if the file is readable, false otherwise.
     */
    bool is_accessible(std::string file)
    {
        std::ifstream fin(file);
        return fin.good();
    }

    /**
     * Check if the given string is a valid file name.
     * 
     * @param name
     * 
     * @return True if the file name is valid, false otherwise.
     */
    bool isValidFileName(std::string name)
    {
        return (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_-.") == std::string::npos);
    }

    /**
     * Detect the delimiter used for a CSV file.
     * Detectable delimiters are comma, semicolon, space, and tab.
     * 
     * @param file The csv file to detect the delimiter of.
     * 
     * @return the delimiter used in the file.
     * 
     * @throws std::runtime_error If the file could not be opened.
     */
    char detect_delimiter(std::string file)
    {
        // Open the given file, read the first line
        std::ifstream fin;
        fin.exceptions(std::ofstream::badbit);
        try 
        {
            fin.open(file);
        }
        catch (const std::ifstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + file);
        }

        std::string line;
        std::getline(fin, line);

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

    /**
     * Returns the truncated name of the file.
     * 
     * @param file The file to truncate.
     * 
     * @return The truncated name of the file.
     */
    std::string name_file(std::string file, std::string given_name)
    {
        if (file == "")
        {
            return "";
        }
        if (given_name != "")
        {
            return given_name;
        }

        auto start_index = file.find_last_of("/|\\") + 1;
        auto end_index = file.find_last_of(".");
        auto file_name = file.substr(start_index, end_index - start_index);
        return file_name;
    }

    /**
     * Names the folder to store the output files in, based on the input files and time.
     * 
     * @param g_name The name of the first graph file.
     * @param h_name The name of the second graph file.
     * @param time The time of the run.
     * 
     * @return The name of the folder to store the output files in.
     * 
     * @throws std::runtime_error If a necessary folder/file could not be created.
     */
    std::string name_folder(std::string g_name, std::string h_name, std::string datetime, bool do_timestamp, bool do_greekstamp, std::string alpha, std::string beta, bool do_bio)
    {
        std::string folder = "";

        #ifdef _WIN32 // Windows
        {
            const std::string OUTPUT_FOLDER = "alignments\\";

            // Make directory "alignments\" if it doesn't exist
            if (mkdir(OUTPUT_FOLDER.c_str()) == -1)
            {
                if(errno != EEXIST)
                {
                    throw std::runtime_error("Unable to create directory " + OUTPUT_FOLDER);
                }
            }

            if (do_timestamp)
            {
                folder = OUTPUT_FOLDER + g_name + "-" + h_name + "-" + datetime + "\\";
            } 
            else
            {
                folder = OUTPUT_FOLDER + g_name + "-" + h_name + "\\";
            }
            
            // Make directory if it doesn't exist
            folder = OUTPUT_FOLDER + g_name + "-" + h_name + "-" + datetime + "\\";
            if (mkdir(folder.c_str()) == -1)
            {
                if(errno != EEXIST)
                {
                    throw std::runtime_error("Unable to create output folder " + folder);
                }
            }
        }
        #else // Unix
        {
            const std::string OUTPUT_FOLDER = "alignments/";

            // Make directory "alignments/" if it doesn't exist
            if (mkdir(OUTPUT_FOLDER.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
            {
                if(errno != EEXIST)
                {
                    throw std::runtime_error("Unable to create directory " + OUTPUT_FOLDER);
                }
            }

            folder = OUTPUT_FOLDER + g_name + "-" + h_name;

            if (do_greekstamp)
            {
                folder += "-a" + alpha;
                if (do_bio)
                {
                    folder += "-b" + beta;
                }
            }

            if (do_timestamp)
            {
                folder += "-" + datetime;
            } 
            
            folder += "/";

            // Make directory if it doesn't exist
            if (mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
            {
                if(errno != EEXIST)
                {
                    throw std::runtime_error("Unable to create output folder " + folder);
                }
            }
        }
        #endif

        // Create log file
        auto log_file = folder + "log.txt";
        std::ofstream log;
        log.exceptions(std::ofstream::badbit);
        try 
        {
            log.open(log_file);
        }
        catch (const std::ofstream::failure& e)
        {
            throw std::runtime_error("Unable to create log file " + log_file);
        }
        log.close();

        return folder;
    }

    /**
     * Outputs the given string to the given file and std::cout.
     * 
     * @param file The file to output to.
     * 
     * @throws std::runtime_error If the file could not be opened.
     */
    void out(std::string file, std::string str)
    {
        std::cout << str << std::flush;

        std::ofstream fout;
        fout.exceptions(std::ofstream::badbit);
        try 
        {
            fout.open(file, std::ios_base::app);
        }
        catch (const std::ofstream::failure& e)
        {
            throw std::runtime_error("Unable to write to file " + file);
        }
        fout << str;
        fout.close();
    }

    /**
     * Outputs the given string to the given file and std::cerr.
     * 
     * @param file The file to output to.
     * 
     * @throws std::runtime_error If the file could not be opened.
     */
    void err(std::string file, std::string str)
    {
        std::string err_str = "ERROR: " + str + "\nPROGRAM TERMINATING\n";
        
        std::cerr << err_str << std::flush;

        std::ofstream fout;
        fout.exceptions(std::ofstream::badbit);
        try 
        {
            fout.open(file, std::ios_base::app);
        }
        catch (const std::ofstream::failure& e)
        {
            throw std::runtime_error("Unable to write to file " + file);
        }
        fout << err_str;
        fout.close();
    }

    /* FILE INPUT */

    /**
     * Parse the given csv adjacency matrix file into a list
     * 
     * @param file_in the graph file to parse.
     * 
     * @return Path to the GraphCrunch-friendly version of the input graph.
     * 
     * @throws std::runtime_error If the file could not be opened, or the output file could not be created.
     */
    std::string graphcrunch_in(std::string file_in, std::string file_out)
    {
        // Identify the delimiter for this csv
        char delim;
        try
        {
            delim = detect_delimiter(file_in);
        }
        catch(const std::runtime_error& e)
        {
            throw e;
        }

        // Open the input file
        std::ifstream fin;
        fin.exceptions(std::ofstream::badbit);
        try 
        {
            fin.open(file_in);
        }
        catch (const std::ifstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + file_in);
        }

        // Parse the file into a matrix
        std::vector<std::vector<double>> matrix;
        std::string line;
        while (std::getline(fin, line))
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
                if (matrix[i][j] != 0)
                {
                    list.push_back({i, j});
                }
            }
        }

        // Open the output file
        std::string out_file = file_out + "_gc.csv";
        std::ofstream outfile;
        outfile.exceptions(std::ofstream::badbit);
        try 
        {
            outfile.open(out_file);
        }
        catch (const std::ofstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + out_file);
        }

        // Write the list to a file
        outfile << (matrix.size() - 1) << std::endl;
        outfile << list.size() << std::endl;
        for (auto &row : list)
        {
            outfile << row[0] << " " << row[1] << std::endl;
        }
        outfile.close();

        return out_file;
    }

    /**
     * Parse the file at the given path into a graph object
     * Require that the file is a CSV
     * 
     * @param file The graph file to parse into a graph object.
     * 
     * @return A graph object representing the graph in the given file.
     * 
     * @throws std::runtime_error If the file could not be opened.
     */
    std::vector<std::vector<unsigned>> file_to_graph(std::string file) 
    {
        // Identify the delimiter for this csv
        char delim;
        try
        {
            delim = detect_delimiter(file);
        }
        catch(const std::runtime_error& e)
        {
            throw e;
        }

        // Open the input file
        std::ifstream fin;
        fin.exceptions(std::ofstream::badbit);
        try 
        {
            fin.open(file);
        }
        catch (const std::ifstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + file);
        }
        
        // Parse the file directly into a matrix
        std::vector<std::vector<double>> matrix;
        std::string line;
        while (std::getline(fin, line))
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

    /**
     * Parse the labels from the given file into a vector.
     * 
     * @param file The file to parse the labels from.
     * 
     * @return The labels from the given file.
     * 
     * @throws std::runtime_error If the file could not be opened.
     */
    std::vector<std::string> parse_labels(std::string file)
    {
        std::ifstream fin;
        fin.exceptions(std::ofstream::badbit);
        try 
        {
            fin.open(file);
        }
        catch (const std::ifstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + file);
        }

        std::vector<std::string> labels;
        std::string line;
        std::getline(fin, line);
        while (std::getline(fin, line))
        {
            std::stringstream ss(line);
            std::string cell;
            std::getline(ss, cell, ',');
            labels.push_back(cell);
        }

        return labels;
    }

    /**
     * Parse the file at the given path into a double vector vector.
     * 
     * @param file The file to parse into a cost matrix.
     * 
     * @return The cost matrix.
     * 
     * @throws std::runtime_error If the file could not be opened.
     */
    std::vector<std::vector<double>> file_to_cost(std::string file)
    {
        // std::ifstream fin;
        // fin.exceptions(std::ofstream::badbit);
        // try 
        // {
        //     fin.open(file);
        // }
        // catch (const std::ifstream::failure& e)
        // {
        //     throw std::runtime_error("Unable to open file " + file);
        // }

        // std::vector<std::vector<double>> costs;
        // std::string line;
        // while (std::getline(fin, line))
        // {
        //     std::istringstream iss(line);
        //     std::vector<double> row;
        //     double val;
        //     while (iss >> val)
        //     {
        //         row.push_back(val);
        //     }
        //     costs.push_back(row);
        // }
        // return costs;

        // Identify the delimiter for this csv
        char delim;
        try
        {
            delim = detect_delimiter(file);
        }
        catch(const std::runtime_error& e)
        {
            throw e;
        }

        // Open the input file
        std::ifstream fin;
        fin.exceptions(std::ofstream::badbit);
        try 
        {
            fin.open(file);
        }
        catch (const std::ifstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + file);
        }
        
        // Parse the file directly into a matrix
        std::vector<std::vector<double>> matrix;
        std::string line;
        while (std::getline(fin, line))
        {
            std::stringstream ss(line);
            std::string cell;
            std::vector<double> row;
            while (std::getline(ss, cell, delim))
            {
                try
                {
                    row.push_back(std::stod(cell));
                    // print
                    out(std::stod(cell));
                }
                catch (std::invalid_argument const&)
                {
                    row.push_back(0);
                }
            }
            matrix.push_back(row);
        }

        return matrix;
    }

    /* FILE OUTPUT */

    /**
     * Write graph to a file.
     * 
     * @param folder The folder to write the file to.
     * @param file_name The name of the file to write the graph to.
     * @param graph_labels The labels for the graph.
     * @param graph The graph to write to the file.
     * 
     * @returns The path to the output file.
     * 
     * @throws std::runtime_error If the file could not be written.
     */
    std::string graph_to_file(std::string folder, std::string file_name,
    std::vector<std::string> graph_labels, std::vector<std::vector<unsigned>> graph)
    {
        std::string filestr = folder + "/" + file_name + ".csv";
        std::ofstream fout;
        fout.exceptions(std::ofstream::badbit);
        try 
        {
            fout.open(filestr);
        }
        catch (const std::ofstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + filestr);
        }

        // Write the labels
        fout << "\"\"";
        for (unsigned i = 0; i < graph_labels.size(); ++i)
        {
            fout << "," << graph_labels[i];
        }
        fout << std::endl;

        // Write the graph
        for (unsigned i = 0; i < graph.size(); ++i)
        {
            fout << graph_labels[i];
            for (unsigned j = 0; j < graph[i].size(); ++j)
            {
                fout << "," << graph[i][j];
            }
            fout << std::endl;
        }

        fout.close();

        return filestr;
    }

    /**
     * Write GDVs to a file.
     * 
     * @param file The file to write the GDVs to.
     * @param labels The labels for the GDVs.
     * @param gdvs The GDVs to write to the file.
     * 
     * @return The path to the output file.
     * 
     * @throws std::runtime_error If the file could not be written.
     */
    std::string gdvs_to_file(std::string file, std::vector<std::string> labels,
        std::vector<std::vector<unsigned>> gdvs)
    {
        std::string filestr = file + "_gdvs.csv";
        std::ofstream fout;
        fout.exceptions(std::ofstream::badbit);
        try 
        {
            fout.open(filestr);
        }
        catch (const std::ofstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + filestr);
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

    /**
     * Write the cost matrix to a file.
     * 
     * @param folder The folder to write the cost matrix to.
     * @param g_labels Labels for the G graph.
     * @param h_labels Labels for the H graph.
     * @param cost The cost matrix to write to the file.
     * 
     * @return The path to the output file.
     * 
     * @throws std::runtime_error If the file could not be written.
     */
    std::string cost_to_file(std::string folder, std::string file, std::vector<std::string> g_labels,
        std::vector<std::string> h_labels, std::vector<std::vector<double>> cost)
    {
        // Create and open the file
        std::string filestr = folder + file;
        std::ofstream fout;
        fout.exceptions(std::ofstream::badbit);
        try 
        {
            fout.open(filestr);
        }
        catch (const std::ofstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + filestr);
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

    /**
     * Write the given alignment to a csv file as a matrix. // COPILOTTED
     * 
     * @param folder The folder to write the alignment to.
     * @param g_labels Labels for the G graph.
     * @param h_labels Labels for the H graph.
     * @param alignment The alignment matrix to write to the file.
     * 
     * @return The path to the output file.
     * 
     * @throws std::runtime_error If the file could not be written.
     */
    std::string alignment_to_matrix_file(std::string folder, std::vector<std::string> g_labels,
        std::vector<std::string> h_labels, std::vector<std::vector<double>> alignment)
    {
        // Create and open the file
        std::string filestr = folder + "alignment_matrix.csv";
        std::ofstream fout;
        fout.exceptions(std::ofstream::badbit);
        try 
        {
            fout.open(filestr);
        }
        catch (const std::ofstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + filestr);
        }

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
                if (alignment[i][j] > 0)
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

        return filestr;
    }

    /**
     * Write the given alignment to a csv file as a list.
     * 
     * @param folder The folder to write the alignment to.
     * @param g_labels Labels for the G graph.
     * @param h_labels Labels for the H graph.
     * @param alignment The alignment matrix to write to the file.
     * 
     * @return The path to the output file.
     * 
     * @throws std::runtime_error If the file could not be written.
     */
    std::string alignment_to_list_file(std::string folder, std::vector<std::string> g_labels,
        std::vector<std::string> h_labels, std::vector<std::vector<double>> alignment)
    {
        // Convert the alignment matrix into a list
        std::vector<std::array<double, 3>> list;
        double net_cost = 0;
        for (unsigned i = 0; i < alignment.size(); ++i)
        {
            for (unsigned j = 0; j < alignment[0].size(); ++j)
            {
                if (alignment[i][j] > 0)
                {
                    net_cost += (1 - alignment[i][j]);
                    list.push_back({(double)i, (double)j, alignment[i][j]});
                }
            }
        }

        // Put the list in order of decreasing similarity score
        std::sort(list.begin(), list.end(),
          [](const std::array<double, 3>& a, const std::array<double, 3>& b)
          {
            return a[2] > b[2];
        });

        // Create and open the file
        std::string filestr = folder + "alignment_list.csv";
        std::ofstream fout;
        fout.exceptions(std::ofstream::badbit);
        try 
        {
            fout.open(filestr);
        }
        catch (const std::ofstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + filestr);
        }

        // Write the list to a file
        fout << net_cost << "," << "" << "," << "" << std::endl;
        for (unsigned i = 0; i < list.size(); ++i)
        {
            fout << g_labels[list[i][0]] << "," << h_labels[list[i][1]] << "," << list[i][2] << std::endl;
        }
        fout.close();

        return filestr;
    }

}