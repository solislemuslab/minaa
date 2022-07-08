// top_sim.cpp
// Topological Similarity Calculator (from GRAAL)
// Reed Nelson

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <fstream>
#include <iostream>
#include <array> 
#include <vector>

#include "orca.h"

namespace top {

    const double O[73] = {
    1, 2, 2, 2, 3, 4, 3, 3, 4, 3, 
    4, 4, 4, 4, 3, 4, 6, 5, 4, 5, 
    6, 6, 4, 4, 4, 5, 7, 4, 6, 6, 
    7, 4, 6, 6, 6, 5, 6, 7, 7, 5, 
    7, 6, 7, 6, 5, 5, 6, 8, 7, 6, 
    6, 8, 6, 9, 5, 6, 4, 6, 6, 7, 
    8, 6, 6, 8, 7, 6, 7, 7, 8, 5, 
    6, 6, 4};
    const double alpha = 0.5;

    std::string G_gdvs_f;                    // the file containing G's gdvs
    std::string H_gdvs_f;                    // the file containing H's gdvs
    std::vector<std::array<int, 73>> G_gdvs; // the GDVs for nodes in G
    std::vector<std::array<int, 73>> H_gdvs; // the GDVs for nodes in H
    int G_order;                             // the number of nodes in G
    int H_order;                             // the number of nodes in H
    int G_max_deg;                           // the highest degree among all the nodes in G
    int H_max_deg;                           // the highest degree among all the nodes in H
    std::vector<std::vector<double>> Cost;   // the cost matrix between G and H

    /*
    * The weight of orbit i, accounting for dependencies between orbits.
    */
    double weight(int i) {
        return 1 - log10(O[i]) / log10(73);
    }

    /*
    * The distance between the ith orbits of nodes v and u.
    */
    double distance(int vi, int ui, int i) {

        //printf("vi: %i; ui: %i;\n", vi, ui); // debug

        double ret;
        ret = log10(vi + 1) - log10(ui + 1);
        ret = abs(ret);
        ret /= log10(std::max(vi, ui) + 2);
        ret *= weight(i);

        return ret;
    }

    /*
    * The signature similarity between nodes v and u. (1 - the distance between v and u).
    */
    double similarity(std::array<int, 73> v, std::array<int, 73> u) {
        double dist = 0;
        for (int i = 0; i < 73; i++) {
            dist += distance(v[i], u[i], i);
        }

        double wei = 0;
        for (int i = 0; i < 73; i++) {
            wei += weight(i);
        }

        return 1 - dist / wei;
    }

    /*
    * The cost of aligning nodes v and u.
    */
    double cost(std::array<int, 73> v, std::array<int, 73> u) {
        double node_degs;
        node_degs = (v[0] + u[0]) / (G_max_deg + H_max_deg);

        return 2 - ((1 - alpha) * node_degs + alpha * similarity(v, u));
    }

    /*
    * The maximum degree of all the nodes in the given graph.
    */
    int max_deg(std::vector<std::array<int, 73>> gdvs, int graph_order) {
        int max = 0;
        for (int i = 0; i < graph_order; i++) { // gdvs.size()
            if (max < gdvs[i][0]) {
                max = gdvs[i][0];
            }
        }

        return max;
    }

    /*
    * Parse the file at the given path into a vector of integer arrays.
    * https://www.cplusplus.com/reference/fstream/ifstream/
    * https://www.internalpointers.com/post/beginner-s-look-smart-pointers-modern-c
    */
    std::vector<std::array<int, 73>> parse(std::string filestr) {

        std::ifstream fin(filestr);
        if (!fin.is_open()) {
            std::cerr << "Could not open file " << filestr << std::endl;
            return {};
        }

        std::vector<std::array<int, 73>> ret;

        for (int i = 0; !fin.eof(); i++) {
            std::array<int, 73> gdv;
            for (int j = 0; j < 73; j++) {
                fin >> gdv[j];
                //printf("%i ", gdv[j]); //debug
                gdv[j] = abs(gdv[j]); // debug
            }
            ret.push_back(gdv);
        }
        ret.pop_back(); // sort of a hack to get rid of the last line, should probably solve more intelligently

        fin.close();

        return ret;
    }

    /*
    * Initialize all uninitialized globals.
    */
    void init(std::string gfin, std::string hfin) {
        // Calculate the graphlet degree vectors for each node of each graph
        G_gdvs_f = orca(gfin);
        H_gdvs_f = orca(hfin);

        // Get the GDVs for each node in G and H
        G_gdvs = parse(G_gdvs_f);
        H_gdvs = parse(H_gdvs_f);

        // Get the number of nodes in G and H
        G_order = G_gdvs.size();
        H_order = H_gdvs.size();

        // Calculate the highest degree among all the nodes in G, H
        G_max_deg = max_deg(G_gdvs, G_order);
        H_max_deg = max_deg(H_gdvs, H_order);

        //printf("G_order: %i; H_order: %i;\n", G_order, H_order); // debug

        // Initialize the cost matrix to the right dimensions
        Cost.resize(G_order);
        for (int i = 0; i < G_order ; i++) {
            Cost[i].resize(H_order);
        }
    }

    /*
    * Write the resulting topological similarity matrix to a file.
    */
    void writeResults(std::vector<std::vector<double>> Cost) {
        std::ofstream fout("top_sim_results.txt");
        if (!fout.is_open()) {
            std::cerr << "Could not open file results.txt" << std::endl;
            return;
        }

        for (int i = 0; i < G_order; i++) {
            for (int j = 0; j < H_order; j++) {
                fout << Cost[i][j] << " ";
            }
            fout << std::endl;
        }

        fout.close();
    }

    /*
    * Calculate the topological similarity between the graphs at the given paths.
    */
    std::string top_sim(std::string gfin, std::string hfin) {
        // Initialize all uninitialized globals
        init(gfin, hfin);

        // Calculate the cost matrix between G and H
        for (int i = 0; i < G_order; i++) {
            for (int j = 0; j < H_order; j++) {
                Cost[i][j] = cost(G_gdvs[i], H_gdvs[j]);
            }
        }

        // Store the cost matrix in a file
        writeResults(Cost);

        return G_gdvs_f; // not this, of course
    }

}
