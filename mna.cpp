// mna.cpp
// Microbial Network Alignment
// Reed Nelson

#include <chrono>
#include <vector>
#include <iostream>
#include <stdexcept>

#include "hungarian.h"
#include "gdvs_dist.h"
#include "graphcrunch.h"
// #include "orca.h"
#include "file_io.h"
#include "util.h"

/*
 * Main function
 */
int main(int argc, char* argv[])
{
    std::string log;
    try
    {
        auto datetime = Util::now();

        // // Tests
        // if (argc == 2)
        // {
        //     // Rscript visualize.R bridge.csv
        //     // ./mna.exe testdata/no_tuber_scab_adj/no_tuber_scab_Net1_1.csv testdata/no_tuber_scab_adj/no_tuber_scab_Net1_2.csv
        //     return 0;
        // }

        // Parse command line arguments
        std::vector<std::string> args;
        try
        {
            args = Util::parse_args(argc, argv);
        }
        catch (std::invalid_argument& e)
        {
            std::cerr << "ERROR: " << e.what() << "\nPROGRAM TERMINATING" << std::endl;
            return 1;
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
        log = folder + "log.txt";

        // Write log file
        FileIO::out(log, "Executing:   ");
        for (auto i = 0; i < argc; ++i)
        {
            FileIO::out(log, argv[i]);
            FileIO::out(log, " ");
        }
        FileIO::out(log, "\n");
        FileIO::out(log, "Y_M_D-H_M_S: " + datetime + "\n");
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

        // Read graph files into graph objects
        FileIO::out(log, "Reading graph files............................");
        auto s00 = std::chrono::high_resolution_clock::now();
        auto g_graph = FileIO::file_to_graph(g_f);
        auto h_graph = FileIO::file_to_graph(h_f);
        auto g_labels = FileIO::parse_labels(g_f);
        auto h_labels = FileIO::parse_labels(h_f);
        auto f00 = std::chrono::high_resolution_clock::now();
        auto d00 = std::chrono::duration_cast<std::chrono::milliseconds>(f00-s00).count();
        FileIO::out(log, "done. (" + std::to_string(d00) + "ms)\n");

        // Calculate the GDVs for G and H
        FileIO::out(log, "Calculating GDVs...............................");
        auto s10 = std::chrono::high_resolution_clock::now();
        auto g_gc_f = FileIO::graphcrunch_in(g_f, folder + g_name); // TEMP
        auto h_gc_f = FileIO::graphcrunch_in(h_f, folder + h_name); // TEMP
        auto g_gdvs = GraphCrunch::graphcrunch(g_gc_f);             // TEMP
        auto h_gdvs = GraphCrunch::graphcrunch(h_gc_f);             // TEMP
        std::string rm1 = "rm " + folder + g_name + ".csv";         // TEMP
        system(rm1.c_str());                                        // TEMP
        std::string rm2 = "rm " + folder + h_name + ".csv";         // TEMP
        system(rm2.c_str());                                        // TEMP
        // auto g_gdvs = ORCA::orca(g_graph);
        // auto h_gdvs = ORCA::orca(h_graph);
        auto f10 = std::chrono::high_resolution_clock::now();
        auto d10 = std::chrono::duration_cast<std::chrono::milliseconds>(f10-s10).count();
        FileIO::out(log, "done. (" + std::to_string(d10) + "ms)\n");

        // Writing GDVs to files
        FileIO::out(log, "Writing GDVs to files..........................");
        auto s11 = std::chrono::high_resolution_clock::now();
        auto g_gdvs_f = FileIO::gdvs_to_file(folder + g_name, g_labels, g_gdvs);
        auto h_gdvs_f = FileIO::gdvs_to_file(folder + h_name, h_labels, h_gdvs);
        auto f11 = std::chrono::high_resolution_clock::now();
        auto d11 = std::chrono::duration_cast<std::chrono::milliseconds>(f11-s11).count();
        FileIO::out(log, "done. (" + std::to_string(d11) + "ms)\n");

        // Calculate the topological similarity matrix
        FileIO::out(log, "Calculating the topological cost matrix........");
        auto s20 = std::chrono::high_resolution_clock::now();
        auto topological_costs = GDVs_Dist::gdvs_dist(g_gdvs, h_gdvs, alpha);
        auto f20 = std::chrono::high_resolution_clock::now();
        auto d20 = std::chrono::duration_cast<std::chrono::milliseconds>(f20-s20).count();
        FileIO::out(log, "done. (" + std::to_string(d20) + "ms)\n");
        
        // Store the topological cost matrix in a file
        FileIO::out(log, "Writing the topological cost matrix to file....");
        auto s21 = std::chrono::high_resolution_clock::now();
        auto topological_costs_f = FileIO::cost_to_file(folder, g_labels, h_labels, topological_costs);
        auto f21 = std::chrono::high_resolution_clock::now();
        auto d21 = std::chrono::duration_cast<std::chrono::milliseconds>(f21-s21).count();
        FileIO::out(log, "done. (" + std::to_string(d21) + "ms)\n");
        
        std::vector<std::vector<double>> alignment;

        if (bio) // incorporate biological data
        {
            // Parse and normalize the biological cost matrix
            FileIO::out(log, "Processing biological data.....................");
            auto s30 = std::chrono::high_resolution_clock::now();
            auto biological_costs = FileIO::file_to_cost(bio_f);
            biological_costs = Util::normalize(biological_costs);
            auto f30 = std::chrono::high_resolution_clock::now();
            auto d30 = std::chrono::duration_cast<std::chrono::milliseconds>(f30-s30).count();
            FileIO::out(log, "done. (" + std::to_string(d30) + "ms)\n");
            
            // Store the biological cost matrix in a file
            FileIO::out(log, "Writing the biological cost matrix to file.....");
            auto s31 = std::chrono::high_resolution_clock::now();
            auto biological_costs_f = FileIO::cost_to_file(folder, g_labels, h_labels, biological_costs);
            auto f31 = std::chrono::high_resolution_clock::now();
            auto d31 = std::chrono::duration_cast<std::chrono::milliseconds>(f31-s31).count();
            FileIO::out(log, "done. (" + std::to_string(d31) + "ms)\n");

            // Calculate the overall cost matrix
            FileIO::out(log, "Calculating the overall cost matrix............");
            auto s40 = std::chrono::high_resolution_clock::now();
            auto overall_costs = Util::combine(topological_costs, biological_costs, beta);
            auto f40 = std::chrono::high_resolution_clock::now();
            auto d40 = std::chrono::duration_cast<std::chrono::milliseconds>(f40-s40).count();
            FileIO::out(log, "done. (" + std::to_string(d40) + "ms)\n");

            // Store the overall cost matrix in a file
            FileIO::out(log, "Writing the overall cost matrix to file........");
            auto s41 = std::chrono::high_resolution_clock::now();
            auto overall_costs_f = FileIO::cost_to_file(folder, g_labels, h_labels, overall_costs);
            auto f41 = std::chrono::high_resolution_clock::now();
            auto d41 = std::chrono::duration_cast<std::chrono::milliseconds>(f41-s41).count();
            FileIO::out(log, "done. (" + std::to_string(d41) + "ms)\n");

            // Run the alignment algorithm
            FileIO::out(log, "Aligning the graphs............................");
            auto s50 = std::chrono::high_resolution_clock::now();
            alignment = Hungarian::hungarian(overall_costs);
            auto f50 = std::chrono::high_resolution_clock::now();
            auto d50 = std::chrono::duration_cast<std::chrono::milliseconds>(f50-s50).count();
            FileIO::out(log, "done. (" + std::to_string(d50) + "ms)\n");
        }
        else // topopogical data only
        {
            // Run the alignment algorithm
            FileIO::out(log, "Aligning the graphs............................");
            auto s50 = std::chrono::high_resolution_clock::now();
            alignment = Hungarian::hungarian(topological_costs);
            auto f50 = std::chrono::high_resolution_clock::now();
            auto d50 = std::chrono::duration_cast<std::chrono::milliseconds>(f50-s50).count();
            FileIO::out(log, "done. (" + std::to_string(d50) + "ms)\n");
        }

        // Write the alignment to csv files
        FileIO::out(log, "Writing the alignment to file..................");
        auto s51 = std::chrono::high_resolution_clock::now();
        auto alignment_mf = FileIO::alignment_to_matrix_file(folder, g_labels, h_labels, alignment, gamma);
        auto alignment_lf = FileIO::alignment_to_list_file(folder, g_labels, h_labels, alignment, gamma);
        auto f51 = std::chrono::high_resolution_clock::now();
        auto d51 = std::chrono::duration_cast<std::chrono::milliseconds>(f51-s51).count();
        FileIO::out(log, "done. (" + std::to_string(d51) + "ms)\n");

        // Use the alignment to bridge G and H
        FileIO::out(log, "Bridging G and H...............................");
        auto s60 = std::chrono::high_resolution_clock::now();
        auto bridged = Util::bridge(g_graph, h_graph, alignment);
        auto f60 = std::chrono::high_resolution_clock::now();
        auto d60 = std::chrono::duration_cast<std::chrono::milliseconds>(f60-s60).count();
        FileIO::out(log, "done. (" + std::to_string(d60) + "ms)\n");

        // Write bridge to file
        FileIO::out(log, "Writing the bridged graphs to file.............");
        auto s61 = std::chrono::high_resolution_clock::now();
        auto bridged_f = FileIO::bridged_to_file(folder, g_labels, h_labels, bridged);
        auto f61 = std::chrono::high_resolution_clock::now();
        auto d61 = std::chrono::duration_cast<std::chrono::milliseconds>(f61-s61).count();
        FileIO::out(log, "done. (" + std::to_string(d61) + "ms)\n");

        // Use the alignment to merge H and G
        FileIO::out(log, "Collapsing G and H.............................");
        auto s70 = std::chrono::high_resolution_clock::now();
        auto merged_labels = Util::merge_labels(alignment, g_labels, h_labels);
        auto merged = Util::merge(g_graph, h_graph, alignment, g_labels, h_labels, merged_labels);
        auto f70 = std::chrono::high_resolution_clock::now();
        auto d70 = std::chrono::duration_cast<std::chrono::milliseconds>(f70-s70).count();
        FileIO::out(log, "done. (" + std::to_string(d70) + "ms)\n");

        // Write merge to file
        FileIO::out(log, "Writing the merged graphs to file..............");
        auto s71 = std::chrono::high_resolution_clock::now();
        auto merged_f = FileIO::merged_to_file(folder, merged_labels, merged);
        auto f71 = std::chrono::high_resolution_clock::now();
        auto d71 = std::chrono::duration_cast<std::chrono::milliseconds>(f71-s71).count();
        FileIO::out(log, "done. (" + std::to_string(d71) + "ms)\n");

        // // Get a visual representation of the alignment using R
        // std::string cmd = "Rscript ./visualize.R " + folder + "bridge.csv";
        // system(cmd.c_str());

        auto f = std::chrono::high_resolution_clock::now();
        auto d = std::chrono::duration_cast<std::chrono::milliseconds>(f-s).count();
        FileIO::out(log, "ALIGNMENT COMPLETED (" + std::to_string(d) + "ms)\n");
    }
    catch (std::exception& e)
    {
        FileIO::out(log, "\nERROR:\n " + std::string(e.what()) + "\nPROGRAM TERMINATING" + "\n");
        return 1;
    }

    return 0;
}