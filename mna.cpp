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
    auto datetime = Util::now();

    // // Tests
    // if (argc == 2)
    // {
    //     // // Run through graphcrunch
    //     // FileIO::graphcrunch_in(argv[1]);
    //     // FileIO::parse_labels(argv[1]);

    //     // // Run through detect_delimiter()
    //     // FileIO::detect_delimiter(argv[1]);

    //     return 0;
    // }

    // Parse command line arguments
    auto args = Util::parse_args(argc, argv);

    if (args[0] == "-1")
    {
        std::cerr << "Usage: mna.exe <G.csv> <H.csv> [-B=bio_costs.csv] [-a=alpha] [-b=beta] [-g=gamma]" << std::endl;
        return 0;
    }

    auto g_f = args[1];              // graph G file
    auto h_f = args[2];              // graph H file
    auto bio_f = args[3];            // biological data file
    auto alpha = std::stod(args[4]); // GDV - edge weight balancer
    auto beta = std::stod(args[5]);  // topological - biological balancer
    auto gamma = std::stod(args[6]); // alignment cost threshold
    
    bool bio = (bio_f != "");        // biological data file provided

    // Generate output names
    auto g_name = FileIO::name_file(g_f);
    auto h_name = FileIO::name_file(h_f);
    auto folder = FileIO::name_folder(g_name, h_name, datetime);
    auto log = folder + "log.txt";

    // Write log file
    FileIO::out(log, "Executing:   ");
    for (auto i = 0; i < argc; ++i)
    {
        FileIO::out(log, argv[i]);
        FileIO::out(log, " ");
    }
    FileIO::out(log, "\n");
    FileIO::out(log, "D_M_Y-H_M_S: " + datetime + "\n");
    FileIO::out(log, "\n");
    FileIO::out(log, "ARGUMENTS\n");
    FileIO::out(log, "G File:   " + g_name + "\n");
    FileIO::out(log, "H File:   " + h_name + "\n");
    FileIO::out(log, "Bio File: " + bio_f + "\n");
    FileIO::out(log, "Alpha:    " + std::to_string(alpha) + "\n");
    FileIO::out(log, "Beta:     " + std::to_string(beta) + "\n");
    FileIO::out(log, "Gamma:    " + std::to_string(gamma) + "\n");
    FileIO::out(log, "\n");

    FileIO::out(log, "BEGINNING ALIGNMENT\n");
    auto s = std::chrono::high_resolution_clock::now();

    // // Read graph files into graph objects [UNCOMMENT WHEN ORCA IS FIXED]
    // auto s00 = std::chrono::high_resolution_clock::now();
    // FileIO::out(log, "[0.0] Reading graph files.........................");
    // auto g_graph = FileIO::file_to_graph(g_f);
    // auto h_graph = FileIO::file_to_graph(h_f);
    // auto g_labels = FileIO::parse_labels(g_f);
    // auto h_labels = FileIO::parse_labels(h_f);
    // auto f00 = std::chrono::high_resolution_clock::now();
    // auto d00 = std::chrono::duration_cast<std::chrono::milliseconds>(f00-s00).count();
    // FileIO::out(log, "done. (" + std::to_string(d00) + "ms)\n");

    // // Calculate the GDVs for G and H [UNCOMMENT WHEN ORCA IS FIXED]
    // auto s10 = std::chrono::high_resolution_clock::now();
    // FileIO::out(log, "[1.0] Calculating GDVs............................");
    // auto g_gdvs = ORCA::orca(g_graph);
    // auto h_gdvs = ORCA::orca(h_graph);
    // auto f10 = std::chrono::high_resolution_clock::now();
    // auto d10 = std::chrono::duration_cast<std::chrono::milliseconds>(f10-s10).count();
    // FileIO::out(log, "done. (" + std::to_string(d10) + "ms)\n");

    // // Writing GDVs to files [UNCOMMENT WHEN ORCA IS FIXED]
    // auto s11 = std::chrono::high_resolution_clock::now();
    // FileIO::out(log, "[1.1] Writing GDVs to files.......................");
    // auto g_gdvs_f = FileIO::gdvs_to_file(folder, g_name, g_labels, g_gdvs);
    // auto h_gdvs_f = FileIO::gdvs_to_file(folder, h_name, h_labels, h_gdvs);
    // auto f11 = std::chrono::high_resolution_clock::now();
    // auto d11 = std::chrono::duration_cast<std::chrono::milliseconds>(f11-s11).count();
    // FileIO::out(log, "done. (" + std::to_string(d11) + "ms)\n");

    // Calculate the GDVs for G and H [DELETE WHEN ORCA IS FIXED]
    FileIO::out(log, "[1.0] Calculating GDVs...............................");
    auto s10 = std::chrono::high_resolution_clock::now();
    auto g_labels = FileIO::parse_labels(g_f);
    auto h_labels = FileIO::parse_labels(h_f);
    auto g_orca_in = FileIO::orca_in(g_f);
    auto h_orca_in = FileIO::orca_in(h_f);
    auto g_gdvs_f = orca(g_orca_in, folder, g_name);
    auto h_gdvs_f = orca(h_orca_in, folder, h_name);
    auto f10 = std::chrono::high_resolution_clock::now();
    auto d10 = std::chrono::duration_cast<std::chrono::milliseconds>(f10-s10).count();
    FileIO::out(log, "done. (" + std::to_string(d10) + "ms)\n");

    // Get the GDVs for each node in G and H [DELETE WHEN ORCA IS FIXED]
    auto s11 = std::chrono::high_resolution_clock::now();
    FileIO::out(log, "[1.1] Reading GDVs from files........................");
    auto g_gdvs = FileIO::file_to_gdvs(g_gdvs_f);
    auto h_gdvs = FileIO::file_to_gdvs(h_gdvs_f);
    auto f11 = std::chrono::high_resolution_clock::now();
    auto d11 = std::chrono::duration_cast<std::chrono::milliseconds>(f11-s11).count();
    FileIO::out(log, "done. (" + std::to_string(d11) + "ms)\n");

    // Calculate the topological similarity matrix
    auto s20 = std::chrono::high_resolution_clock::now();
    FileIO::out(log, "[2.0] Calculating the topological cost matrix........");
    auto topological_costs = GDVs_Dist::gdvs_dist(g_gdvs, h_gdvs, alpha);
    auto f20 = std::chrono::high_resolution_clock::now();
    auto d20 = std::chrono::duration_cast<std::chrono::milliseconds>(f20-s20).count();
    FileIO::out(log, "done. (" + std::to_string(d20) + "ms)\n");
    
    // Store the topological cost matrix in a file
    auto s21 = std::chrono::high_resolution_clock::now();
    FileIO::out(log, "[2.1] Writing the topological cost matrix to file....");
    auto topological_costs_f = FileIO::cost_to_file(folder, g_labels, h_labels, topological_costs);
    auto f21 = std::chrono::high_resolution_clock::now();
    auto d21 = std::chrono::duration_cast<std::chrono::milliseconds>(f21-s21).count();
    FileIO::out(log, "done. (" + std::to_string(d21) + "ms)\n");
    
    if (bio) // incorporate biological data
    {
        // Parse and normalize the biological cost matrix
        auto s30 = std::chrono::high_resolution_clock::now();
        FileIO::out(log, "[3.0] Processing biological data.....................");
        auto biological_costs = FileIO::file_to_cost(bio_f);
        biological_costs = Util::normalize(biological_costs);
        auto f30 = std::chrono::high_resolution_clock::now();
        auto d30 = std::chrono::duration_cast<std::chrono::milliseconds>(f30-s30).count();
        FileIO::out(log, "done. (" + std::to_string(d30) + "ms)\n");
        
        // Store the biological cost matrix in a file
        auto s31 = std::chrono::high_resolution_clock::now();
        FileIO::out(log, "[3.1] Writing the biological cost matrix to file.....");
        auto biological_costs_f = FileIO::cost_to_file(folder, g_labels, h_labels, biological_costs);
        auto f31 = std::chrono::high_resolution_clock::now();
        auto d31 = std::chrono::duration_cast<std::chrono::milliseconds>(f31-s31).count();
        FileIO::out(log, "done. (" + std::to_string(d31) + "ms)\n");

        // Calculate the overall cost matrix
        auto s40 = std::chrono::high_resolution_clock::now();
        FileIO::out(log, "[4.0] Calculating the overall cost matrix............");
        auto overall_costs = Util::merge(topological_costs, biological_costs, beta);
        auto f40 = std::chrono::high_resolution_clock::now();
        auto d40 = std::chrono::duration_cast<std::chrono::milliseconds>(f40-s40).count();
        FileIO::out(log, "done. (" + std::to_string(d40) + "ms)\n");

        // Store the overall cost matrix in a file
        auto s41 = std::chrono::high_resolution_clock::now();
        FileIO::out(log, "[4.1] Writing the overall cost matrix to file........");
        auto overall_costs_f = FileIO::cost_to_file(folder, g_labels, h_labels, overall_costs);
        auto f41 = std::chrono::high_resolution_clock::now();
        auto d41 = std::chrono::duration_cast<std::chrono::milliseconds>(f41-s41).count();
        FileIO::out(log, "done. (" + std::to_string(d41) + "ms)\n");

        // Run the alignment algorithm
        auto s50 = std::chrono::high_resolution_clock::now();
        FileIO::out(log, "[5.0] Aligning the graphs............................");
        auto alignment = Hungarian::hungarian(overall_costs);
        auto f50 = std::chrono::high_resolution_clock::now();
        auto d50 = std::chrono::duration_cast<std::chrono::milliseconds>(f50-s50).count();
        FileIO::out(log, "done. (" + std::to_string(d50) + "ms)\n");

        // Write the results to a csv file
        auto s51 = std::chrono::high_resolution_clock::now();
        FileIO::out(log, "[5.1] Writing the alignment to file..................");
        auto alignment_mf = FileIO::alignment_to_matrix_file(folder, g_labels, h_labels, alignment, gamma);
        auto alignment_lf = FileIO::alignment_to_list_file(folder, g_labels, h_labels, alignment, gamma);
        auto f51 = std::chrono::high_resolution_clock::now();
        auto d51 = std::chrono::duration_cast<std::chrono::milliseconds>(f51-s51).count();
        FileIO::out(log, "done. (" + std::to_string(d51) + "ms)\n");
    }
    else // topopogical data only
    {
        // Run the alignment algorithm
        auto s30 = std::chrono::high_resolution_clock::now();
        FileIO::out(log, "[3.0] Aligning the graphs............................");
        auto alignment = Hungarian::hungarian(topological_costs);
        auto f30 = std::chrono::high_resolution_clock::now();
        auto d30 = std::chrono::duration_cast<std::chrono::milliseconds>(f30-s30).count();
        FileIO::out(log, "done. (" + std::to_string(d30) + "ms)\n");

        // Write the results to a csv file
        auto s31 = std::chrono::high_resolution_clock::now();
        FileIO::out(log, "[3.1] Writing the alignment to file..................");
        auto alignment_mf = FileIO::alignment_to_matrix_file(folder, g_labels, h_labels, alignment, gamma);
        auto alignment_lf = FileIO::alignment_to_list_file(folder, g_labels, h_labels, alignment, gamma);
        auto f31 = std::chrono::high_resolution_clock::now();
        auto d31 = std::chrono::duration_cast<std::chrono::milliseconds>(f31-s31).count();
        FileIO::out(log, "done. (" + std::to_string(d31) + "ms)\n");
    }

    auto f = std::chrono::high_resolution_clock::now();
    auto d = std::chrono::duration_cast<std::chrono::milliseconds>(f-s).count();
    FileIO::out(log, "ALIGNMENT COMPLETED (" + std::to_string(d) + "ms)\n");

    return 0;
}