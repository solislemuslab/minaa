// mna.cpp
// Microbial Network Alignment
// Reed Nelson

#include <array>
#include <chrono>
#include <vector>
#include <iostream>

#include "hungarian.h"
#include "gdvs_dist.h"
#include "orca.h"
#include "file_io.h"
#include "util.h"

/*
 * char* input file 1
 * char* input file 2
 */
int main(int argc, char* argv[])
{
    if (argc < 3 || argc > 4)
    {
        std::cerr << "Usage: mna.exe <graph G> <graph H> [biological data]" << std::endl;
        return 0;
    }

    auto g_f = std::string(argv[1]);  // graph G file
    auto h_f = std::string(argv[2]);  // graph H file

    bool bio = false;
    std::string bio_f = "";
    double alpha = -1;

    if (argc == 4)
    {
        bio = true;
        bio_f = std::string(argv[3]);  // biological data file
        alpha = 0.5;
    }

    // Generate output names
    auto g_name = FileIO::name_file(g_f);
    auto h_name = FileIO::name_file(h_f);
    auto folder = FileIO::name_folder(g_name, h_name);

    std::cout << "G: " << g_name << "; H: " << h_name << "; Folder: " << folder << ";" << std::endl; // DEBUG

    // s = start time
    // f = finish time
    // d = duration
    auto s = std::chrono::high_resolution_clock::now();

    // // Read graph files into graph objects [UNCOMMENT WHEN ORCA IS FIXED]
    // auto s00 = std::chrono::high_resolution_clock::now();
    // std::cout << "[0.0] Reading graph files.........................";
    // auto g_graph = FileIO::file_to_graph(g_f);
    // auto h_graph = FileIO::file_to_graph(h_f);
    // auto f00 = std::chrono::high_resolution_clock::now();
    // auto d00 = std::chrono::duration_cast<std::chrono::milliseconds>(f00-s00).count();
    // std::cout << "done. (" << d00 << "ms)" <<  std::endl;

    // // Calculate the GDVs for G and H [UNCOMMENT WHEN ORCA IS FIXED]
    // auto s10 = std::chrono::high_resolution_clock::now();
    // std::cout << "[1.0] Calculating GDVs............................";
    // auto g_gdvs = ORCA::orca(g_graph);
    // auto h_gdvs = ORCA::orca(h_graph);
    // auto f10 = std::chrono::high_resolution_clock::now();
    // auto d10 = std::chrono::duration_cast<std::chrono::milliseconds>(f10-s10).count();
    // std::cout << "done. (" << d10 << "ms)" <<  std::endl;

    // // Writing GDVs to files [UNCOMMENT WHEN ORCA IS FIXED]
    // auto s11 = std::chrono::high_resolution_clock::now();
    // std::cout << "[1.1] Writing GDVs to files.......................";
    // auto g_gdvs_f = FileIO::gdvs_to_file(folder, g_name, g_gdvs);
    // auto h_gdvs_f = FileIO::gdvs_to_file(folder, h_name, h_gdvs);
    // auto f11 = std::chrono::high_resolution_clock::now();
    // auto d11 = std::chrono::duration_cast<std::chrono::milliseconds>(f11-s11).count();
    // std::cout << "done. (" << d11 << "ms)" <<  std::endl;

    // Calculate the GDVs for G and H [DELETE WHEN ORCA IS FIXED]
    std::cout << "[1.0] Calculating GDVs...............................";
    auto s10 = std::chrono::high_resolution_clock::now();
    auto g_gdvs_f = orca(g_name);
    auto h_gdvs_f = orca(h_name);
    auto f10 = std::chrono::high_resolution_clock::now();
    auto d10 = std::chrono::duration_cast<std::chrono::milliseconds>(f10-s10).count();
    std::cout << "done. (" << d10 << "ms)" <<  std::endl;

    // Get the GDVs for each node in G and H [DELETE WHEN ORCA IS FIXED]
    auto s11 = std::chrono::high_resolution_clock::now();
    std::cout << "[1.1] Read GDVs from files...........................";
    auto g_gdvs = FileIO::file_to_gdvs(g_gdvs_f);
    auto h_gdvs = FileIO::file_to_gdvs(h_gdvs_f);
    auto f11 = std::chrono::high_resolution_clock::now();
    auto d11 = std::chrono::duration_cast<std::chrono::milliseconds>(f11-s11).count();
    std::cout << "done. (" << d11 << "ms)" <<  std::endl;

    // Calculate the topological similarity matrix
    auto s20 = std::chrono::high_resolution_clock::now();
    std::cout << "[2.0] Calculating the topological cost matrix........";
    auto topological_costs = GDVs_Dist::gdvs_dist(g_gdvs, h_gdvs);
    auto f20 = std::chrono::high_resolution_clock::now();
    auto d20 = std::chrono::duration_cast<std::chrono::milliseconds>(f20-s20).count();
    std::cout << "done. (" << d20 << "ms)" <<  std::endl;
    
    // Store the topological cost matrix in a file
    auto s21 = std::chrono::high_resolution_clock::now();
    std::cout << "[2.1] Writing the topological cost matrix to file....";
    auto topological_costs_f = FileIO::cost_to_file(folder, topological_costs);
    auto f21 = std::chrono::high_resolution_clock::now();
    auto d21 = std::chrono::duration_cast<std::chrono::milliseconds>(f21-s21).count();
    std::cout << "done. (" << d21 << "ms)" <<  std::endl;
    
    if (bio) // incorporate biological data
    {
        // Incorporate a biological cost matrix
        auto s30 = std::chrono::high_resolution_clock::now();
        std::cout << "[3.0] Processing biological data.....................";
        auto biological_costs = FileIO::file_to_cost(bio_f);
        auto f30 = std::chrono::high_resolution_clock::now();
        auto d30 = std::chrono::duration_cast<std::chrono::milliseconds>(f30-s30).count();
        std::cout << "done. (" << d30 << "ms)" <<  std::endl;
        
        // Store the biological cost matrix in a file
        auto s31 = std::chrono::high_resolution_clock::now();
        std::cout << "[3.1] Writing the biological cost matrix to file...";
        auto biological_costs_f = FileIO::cost_to_file(folder, biological_costs);
        auto f31 = std::chrono::high_resolution_clock::now();
        auto d31 = std::chrono::duration_cast<std::chrono::milliseconds>(f31-s31).count();
        std::cout << "done. (" << d31 << "ms)" <<  std::endl;

        // Calculate the overall cost matrix
        auto s40 = std::chrono::high_resolution_clock::now();
        std::cout << "[4.0] Calculating the overall cost matrix............";
        auto overall_costs = Util::merge(topological_costs, biological_costs, alpha);
        auto f40 = std::chrono::high_resolution_clock::now();
        auto d40 = std::chrono::duration_cast<std::chrono::milliseconds>(f40-s40).count();
        std::cout << "done. (" << d40 << "ms)" <<  std::endl;

        // Store the overall cost matrix in a file
        auto s41 = std::chrono::high_resolution_clock::now();
        std::cout << "[4.1] Writing the overall cost matrix to file........";
        auto overall_costs_f = FileIO::cost_to_file(folder, overall_costs);
        auto f41 = std::chrono::high_resolution_clock::now();
        auto d41 = std::chrono::duration_cast<std::chrono::milliseconds>(f41-s41).count();
        std::cout << "done. (" << d41 << "ms)" <<  std::endl;

        // Run the alignment algorithm
        auto s50 = std::chrono::high_resolution_clock::now();
        std::cout << "[5.0] Aligning the graphs............................";
        auto alignment = Hungarian::hungarian(overall_costs);
        auto f50 = std::chrono::high_resolution_clock::now();
        auto d50 = std::chrono::duration_cast<std::chrono::milliseconds>(f50-s50).count();
        std::cout << "done. (" << d50 << "ms)" <<  std::endl;

        // Write the results to a csv file
        auto s51 = std::chrono::high_resolution_clock::now();
        std::cout << "[5.1] Writing the alignment to file..................";
        auto alignment_f = FileIO::alignment_to_file(folder, alignment);
        auto f51 = std::chrono::high_resolution_clock::now();
        auto d51 = std::chrono::duration_cast<std::chrono::milliseconds>(f51-s51).count();
        std::cout << "done. (" << d51 << "ms)" <<  std::endl;
    }
    else // topopogical data only
    {
        // Run the alignment algorithm
        auto s30 = std::chrono::high_resolution_clock::now();
        std::cout << "[3.0] Aligning the graphs............................";
        auto alignment = Hungarian::hungarian(topological_costs);
        auto f30 = std::chrono::high_resolution_clock::now();
        auto d30 = std::chrono::duration_cast<std::chrono::milliseconds>(f30-s30).count();
        std::cout << "done. (" << d30 << "ms)" <<  std::endl;

        // Write the results to a csv file
        auto s31 = std::chrono::high_resolution_clock::now();
        std::cout << "[3.1] Writing the alignment to file..................";
        auto alignment_f = FileIO::alignment_to_file(folder, alignment);
        auto f31 = std::chrono::high_resolution_clock::now();
        auto d31 = std::chrono::duration_cast<std::chrono::milliseconds>(f31-s31).count();
        std::cout << "done. (" << d31 << "ms)" <<  std::endl;
    }

    auto f = std::chrono::high_resolution_clock::now();
    auto d = std::chrono::duration_cast<std::chrono::milliseconds>(f-s).count();
    std::cout << "ALIGNMENT COMPLETED (" << d << "ms)" <<  std::endl;

    return 0;
}