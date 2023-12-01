#ifndef FILEIO_H  
#define FILEIO_H

namespace FileIO
{
    bool is_accessible(std::string);
    bool isValidFileName(std::string);
    char detect_delimiter(std::string);
    std::string name_file(std::string, std::string);
    std::string name_directory(std::string, std::string, std::string, std::string, bool, bool, std::string, std::string, bool);
    void out(std::string, std::string);
    void err(std::string, std::string);

    std::string graphcrunch_in(std::string, std::string);

    std::vector<std::vector<double>> file_to_matrix(std::string);
    std::vector<std::string> parse_labels(std::string);

    void graph_to_file(std::string, std::vector<std::string>, std::vector<std::vector<unsigned>>);
    void gdvs_to_file(std::string, std::vector<std::string>, std::vector<std::vector<unsigned>>);
    void matrix_to_file(std::string, std::vector<std::string>, std::vector<std::string>, std::vector<std::vector<double>>);
    void alignment_to_matrix_file(std::string, std::vector<std::string>, std::vector<std::string>, std::vector<std::vector<double>>);
    void alignment_to_list_file(std::string, std::vector<std::string>, std::vector<std::string>, std::vector<std::vector<double>>);
}

#endif