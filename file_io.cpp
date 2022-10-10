// file_io.cpp
// File IO Utilities
// Reed Nelson

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <sys/stat.h>

namespace FileIO
{
    /*
     * Returns the truncated name of the file. // MAKE ROBUST
     */
    std::string name_file(std::string file)
    {
        auto si = file.find_last_of("/") + 1;
        auto ei = file.find_last_of(".");
        auto file_name = file.substr(si, ei - si);
        return file_name;
    }

    /*
     * Returns the name of the folder to store the output files in. // MAKE ROBUST
     */
    std::string name_folder(std::string g_name, std::string h_name)
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
        auto folder = OUTPUT_FOLDER + g_name + "-" + h_name + "/";
        if (mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
        {
            if(errno != EEXIST)
            {
                std::cout << "Error: could not create output folder." << std::endl;
                return "";
            }
        }

        return folder;
    }

    /*
     * Parse the file at the given path into a graph object
     */
    std::vector<std::vector<unsigned>> file_to_graph(std::string) {
        std::vector<std::vector<unsigned>> graph;
        return graph;
    }

    // orca: graph to gdvs

    /*
     * Write GDVs to a file
     */
    std::string gdvs_to_file(std::string folder, std::string graph_name, std::vector<std::array<unsigned, 73>> gdvs)
    {
        std::ofstream file(folder + graph_name + "_gdvs.out");
        for (auto gdv : gdvs)
        {
            for (auto i : gdv)
            {
                file << i << ",";
            }
            file << std::endl;
        }
        file.close();
        return folder + graph_name + "_gdvs.out";
    }

    /*
     * Parse the file at the given path into a vector of integer arrays.
     */
    std::vector<std::array<unsigned, 73>> file_to_gdvs(std::string filestr)
    {
        std::ifstream fin(filestr);
        if (!fin.is_open())
        {
            std::cerr << "Could not open file " << filestr << std::endl;
            return {};
        }

        std::vector<std::array<unsigned, 73>> ret;

        for (unsigned i = 0; !fin.eof(); ++i)
        {
            std::array<unsigned, 73> gdv;
            for (unsigned j = 0; j < 73; ++j)
            {
                int temp;
                fin >> temp;
                //printf("%i ", gdv[j]); //debug
                gdv[j] = abs(temp); // ONLY BECAUSE ORCA IS BROKEN (CAN'T TRUST TO BE POSITIVE)
            }
            ret.push_back(gdv);
        }
        ret.pop_back(); // sort of a hack to get rid of the last line, should probably solve more intelligently

        fin.close();

        return ret;
    }

    // top_sim: gdvs to (topological) cost

    /*
     * Write the resulting topological similarity matrix to a file.
     */
    std::string cost_to_file(std::string folder, std::vector<std::vector<double>> cost)
    {
        std::string filestr = "top_costs.out";

        std::ofstream fout(folder + filestr);
        if (!fout.is_open())
        {
            std::cerr << "Could not open file " << filestr << std::endl;
            return "";
        }

        for (unsigned i = 0; i < cost.size(); ++i)
        {
            for (unsigned j = 0; j < cost[i].size(); ++j)
            {
                fout << cost[i][j] << " ";
            }
            fout << std::endl;
        }

        fout.close();

        return filestr;
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

    // align: cost to alignment

    /*
     * Write the given alignment to a csv file.
     */
    std::string alignment_to_file(std::string folder, std::vector<std::array<unsigned, 2>> alignment)
    {
        std::ofstream file(folder + "alignment.csv");
        for (auto &arr : alignment)
        {
            file << arr[0] << "," << arr[1]+100 << std::endl; // BANDAID FIX!
        }
        file.close();

        return "alignment.csv";
    }

}