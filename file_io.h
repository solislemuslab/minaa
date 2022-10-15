#ifndef FILEIO_H  
#define FILEIO_H

namespace FileIO
{
    std::string name_file(std::string);
    std::string name_folder(std::string, std::string);
    std::string orca_in(std::string);
    std::vector<std::vector<unsigned>> file_to_graph(std::string);
    std::string gdvs_to_file(std::string, std::string, std::vector<std::array<unsigned, 73>>);
    std::vector<std::array<unsigned, 73>> file_to_gdvs(std::string); 
    std::string cost_to_file(std::string, std::vector<std::vector<double>>);
    std::vector<std::vector<double>> file_to_cost(std::string);
    std::string alignment_to_file(std::string, std::vector<std::array<unsigned, 2>>);
}

#endif