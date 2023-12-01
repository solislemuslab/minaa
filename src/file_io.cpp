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
     * Returns whether or not the given string is a path to a readable file.
     * 
     * @param filepath The filepath to check.
     * 
     * @return True if the file is readable, false otherwise.
     */
    bool is_accessible(std::string filepath)
    {
        std::ifstream fin(filepath);
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
     * @param filepath Path to the csv file to detect the delimiter of.
     * 
     * @return the delimiter used in the file.
     * 
     * @throws std::runtime_error If the file could not be opened.
     */
    char detect_delimiter(std::string filepath)
    {
        // Open the given file, read the first line
        std::ifstream fin;
        fin.exceptions(std::ofstream::badbit);
        try 
        {
            fin.open(filepath);
        }
        catch (const std::ifstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + filepath);
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
     * @param filepath The filepath to truncate.
     * @param given_name The name to give the file, if nonempty.
     * 
     * @return given_name if nonempty, else the truncated name of the file.
     */
    std::string name_file(std::string filepath, std::string given_name)
    {
        if (filepath == "")
        {
            return "";
        }
        if (given_name != "")
        {
            return given_name;
        }

        auto start_index = filepath.find_last_of("/|\\") + 1;
        auto end_index = filepath.find_last_of(".");
        auto file_name = filepath.substr(start_index, end_index - start_index);
        return file_name;
    }

    /**
     * Names the directory to store the output files in, based on the input files and time.
     * 
     * @param base_path The directory in which the named directory will be created.
     * @param g_name The name of the first graph file.
     * @param h_name The name of the second graph file.
     * @param datetime The date and time of the run.
     * @param do_timestamp Whether or not to include the date and time in the directory name.
     * @param do_greekstamp Whether or not to include the greek parameters in the directory name.
     * @param alpha The alpha value used in the run.
     * @param beta The beta value used in the run.
     * @param do_bio Whether or not a biological input was included.
     * 
     * @return The name of the directory to store the output files in.
     * 
     * @throws std::runtime_error If a necessary directory/file could not be created.
     */
    std::string name_directory(std::string base_path, std::string g_name, std::string h_name, std::string datetime, bool do_timestamp, bool do_greekstamp, std::string alpha, std::string beta, bool do_bio)
    {
        std::string path = "";

        #ifdef _WIN32 // Windows
        {
            // Make base_path directory if it doesn't exist
            if (mkdir(base_path.c_str()) == -1)
            {
                if(errno != EEXIST)
                {
                    throw std::runtime_error("Unable to create directory " + base_path);
                }
            }

            path = base_path + "\\" + g_name + "-" + h_name;

            if (do_greekstamp)
            {
                path += "-a" + alpha;
                if (do_bio)
                {
                    path += "-b" + beta;
                }
            }

            if (do_timestamp)
            {
                path += "-" + datetime;
            } 
            
            path += "\\";
            
            // Make directory if it doesn't exist
            if (mkdir(path.c_str()) == -1)
            {
                if(errno != EEXIST)
                {
                    throw std::runtime_error("Unable to create output directory " + path);
                }
            }
        }
        #else // Unix
        {
            // Make base_path directory if it doesn't exist
            if (mkdir(base_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
            {
                if(errno != EEXIST)
                {
                    throw std::runtime_error("Unable to create directory " + base_path);
                }
            }

            path = base_path + "/" + g_name + "-" + h_name;

            if (do_greekstamp)
            {
                path += "-a" + alpha;
                if (do_bio)
                {
                    path += "-b" + beta;
                }
            }

            if (do_timestamp)
            {
                path += "-" + datetime;
            } 
            
            path += "/";

            // Make directory if it doesn't exist
            if (mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
            {
                if(errno != EEXIST)
                {
                    throw std::runtime_error("Unable to create output directory " + path);
                }
            }
        }
        #endif

        // Create log file
        auto log_file = path + "log.txt";
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

        return path;
    }

    /**
     * Outputs the given string to the given file and std::cout.
     * 
     * @param filepath The path to the file to write to.
     * 
     * @throws std::runtime_error If the file could not be opened.
     */
    void out(std::string filepath, std::string str)
    {
        std::cout << str << std::flush;

        std::ofstream fout;
        fout.exceptions(std::ofstream::badbit);
        try 
        {
            fout.open(filepath, std::ios_base::app);
        }
        catch (const std::ofstream::failure& e)
        {
            throw std::runtime_error("Unable to write to file " + filepath);
        }
        fout << str;
        fout.close();
    }

    /**
     * Outputs the given string to the given file and std::cerr.
     * 
     * @param filepath The path to the file to write to.
     * 
     * @throws std::runtime_error If the file could not be opened.
     */
    void err(std::string filepath, std::string str)
    {
        std::string err_str = "ERROR: " + str + "\nPROGRAM TERMINATING\n";
        
        std::cerr << err_str << std::flush;

        std::ofstream fout;
        fout.exceptions(std::ofstream::badbit);
        try 
        {
            fout.open(filepath, std::ios_base::app);
        }
        catch (const std::ofstream::failure& e)
        {
            throw std::runtime_error("Unable to write to file " + filepath);
        }
        fout << err_str;
        fout.close();
    }

    /* FILE INPUT */

    /**
     * Parse the given csv adjacency matrix file into a list
     * 
     * @param filepath_in the path to the input file.
     * @param filepath_out the path to the output file.
     * 
     * @return Path to the GraphCrunch-friendly version of the input graph.
     * 
     * @throws std::runtime_error If the file could not be opened, or the output file could not be created.
     */
    std::string graphcrunch_in(std::string filepath_in, std::string filepath_out)
    {
        // Identify the delimiter for this csv
        char delim;
        try
        {
            delim = detect_delimiter(filepath_in);
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
            fin.open(filepath_in);
        }
        catch (const std::ifstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + filepath_in);
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
        std::string out_filepath = filepath_out + "_gc.csv";
        std::ofstream outfile;
        outfile.exceptions(std::ofstream::badbit);
        try 
        {
            outfile.open(out_filepath);
        }
        catch (const std::ofstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + out_filepath);
        }

        // Write the list to a file
        outfile << (matrix.size() - 1) << std::endl;
        outfile << list.size() << std::endl;
        for (auto &row : list)
        {
            outfile << row[0] << " " << row[1] << std::endl;
        }
        outfile.close();

        return out_filepath;
    }

    /**
     * Parse the file at the given path into a matrix.
     * Require that the file is a CSV, and that the first row and column are labels.
     * 
     * @param filepath The graph file to parse into a matrix.
     * 
     * @return A matrix containing the data elements of the given file.
     * 
     * @throws std::runtime_error If the file could not be opened.
     */
    std::vector<std::vector<double>> file_to_matrix(std::string filepath) 
    {
        // Identify the delimiter for this csv
        char delim;
        try
        {
            delim = detect_delimiter(filepath);
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
            fin.open(filepath);
        }
        catch (const std::ifstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + filepath);
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

        return matrix;
    }

    /**
     * Parse the labels from the given file into a vector.
     * 
     * @param filepath The file to parse the labels from.
     * 
     * @return The labels from the given file.
     * 
     * @throws std::runtime_error If the file could not be opened.
     */
    std::vector<std::string> parse_labels(std::string filepath)
    {
        std::ifstream fin;
        fin.exceptions(std::ofstream::badbit);
        try 
        {
            fin.open(filepath);
        }
        catch (const std::ifstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + filepath);
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

    /* FILE OUTPUT */

    /**
     * Write graph to a file.
     * 
     * @param filepath The path to the file to write the graph to.
     * @param labels The labels for the graph.
     * @param graph The graph to write to the file.
     * 
     * @throws std::runtime_error If the file could not be written.
     */
    void graph_to_file(std::string filepath, std::vector<std::string> labels, std::vector<std::vector<unsigned>> graph)
    {
        std::ofstream fout;
        fout.exceptions(std::ofstream::badbit);
        try 
        {
            fout.open(filepath);
        }
        catch (const std::ofstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + filepath);
        }

        // Write the labels
        fout << "\"\"";
        for (unsigned i = 0; i < labels.size(); ++i)
        {
            fout << "," << labels[i];
        }
        fout << std::endl;

        // Write the graph
        for (unsigned i = 0; i < graph.size(); ++i)
        {
            fout << labels[i];
            for (unsigned j = 0; j < graph[i].size(); ++j)
            {
                fout << "," << graph[i][j];
            }
            fout << std::endl;
        }

        fout.close();
    }

    /**
     * Write GDVs to a file.
     * 
     * @param filepath The path to the file to write the GDVs to.
     * @param labels The labels for the GDVs.
     * @param gdvs The GDVs to write to the file.
     * 
     * @throws std::runtime_error If the file could not be written.
     */
    void gdvs_to_file(std::string filepath, std::vector<std::string> labels, std::vector<std::vector<unsigned>> gdvs)
    {
        std::ofstream fout;
        fout.exceptions(std::ofstream::badbit);
        try 
        {
            fout.open(filepath);
        }
        catch (const std::ofstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + filepath);
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
    }

    /**
     * Write the cost matrix to a file.
     * 
     * @param filepath The path to the file to write the cost matrix to.
     * @param g_labels Labels for the G graph.
     * @param h_labels Labels for the H graph.
     * @param matrix The matrix to write to the file.
     * 
     * @throws std::runtime_error If the file could not be written.
     */
    void matrix_to_file(std::string filepath, std::vector<std::string> g_labels, std::vector<std::string> h_labels,
        std::vector<std::vector<double>> matrix)
    {
        // Create and open the file
        std::ofstream fout;
        fout.exceptions(std::ofstream::badbit);
        try 
        {
            fout.open(filepath);
        }
        catch (const std::ofstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + filepath);
        }

        fout << "\"\"";
        for (unsigned i = 0; i < matrix[0].size(); ++i)
        {
            fout << "," << h_labels[i];
        }
        for (unsigned i = 0; i < matrix.size(); ++i)
        {
            fout << std::endl << g_labels[i];
            for (unsigned j = 0; j < matrix[i].size(); ++j)
            {
                fout << "," << matrix[i][j];
            }
        }

        fout.close();
    }

    /**
     * Write the given alignment to a csv file as a matrix. // COPILOTTED
     * 
     * @param filepath The path to the output file.
     * @param g_labels Labels for the G graph.
     * @param h_labels Labels for the H graph.
     * @param alignment The alignment matrix to write to the file.
     * 
     * @throws std::runtime_error If the file could not be written.
     */
    void alignment_to_matrix_file(std::string filepath, std::vector<std::string> g_labels,
        std::vector<std::string> h_labels, std::vector<std::vector<double>> alignment)
    {
        // Create and open the file
        std::ofstream fout;
        fout.exceptions(std::ofstream::badbit);
        try 
        {
            fout.open(filepath);
        }
        catch (const std::ofstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + filepath);
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
    }

    /**
     * Write the given alignment to a csv file as a list.
     * 
     * @param filepath The path to the output file.
     * @param g_labels Labels for the G graph.
     * @param h_labels Labels for the H graph.
     * @param alignment The alignment matrix to write to the file.
     * 
     * @throws std::runtime_error If the file could not be written.
     */
    void alignment_to_list_file(std::string filepath, std::vector<std::string> g_labels,
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
        std::ofstream fout;
        fout.exceptions(std::ofstream::badbit);
        try 
        {
            fout.open(filepath);
        }
        catch (const std::ofstream::failure& e)
        {
            throw std::runtime_error("Unable to open file " + filepath);
        }

        // Write the list to a file
        fout << net_cost << "," << "" << "," << "" << std::endl;
        for (unsigned i = 0; i < list.size(); ++i)
        {
            fout << g_labels[list[i][0]] << "," << h_labels[list[i][1]] << "," << list[i][2] << std::endl;
        }

        fout.close();
    }

}