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

using namespace std;

const double O[73] = {1, 2, 2, 2, 3, 4, 3, 3, 4, 3, 4, 4, 4, 4, 3, 4, 6, 5, 4, 5, 6, 6, 4, 4, 4, 5, 7, 4, 6, 6, 7, 4, 6, 6, 6, 5, 6, 7, 7, 5, 7, 6, 7, 6, 5, 5, 6, 8, 7, 6, 6, 8, 6, 9, 5, 6, 4, 6, 6, 7, 8, 6, 6, 8, 7, 6, 7, 7, 8, 5, 6, 6, 4};
const double alpha = 0.5;

string G_gdvs_f; // the file containing G's gdvs
string H_gdvs_f; // the file containing H's gdvs
std::vector<std::array<int, 73>> G_gdvs;  // the GDVs for nodes in G
std::vector<std::array<int, 73>> H_gdvs;  // the GDVs for nodes in H
int G_order;     // the number of nodes in G
int H_order;     // the number of nodes in H
int G_max_deg;   // the highest degree among all the nodes in G
int H_max_deg;   // the highest degree among all the nodes in G

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
    double ret;
    ret = log10(vi + 1) - log10(ui + 1);
    ret = abs(ret);
    ret /= log10(max(vi, ui) + 2);
    ret *= weight(i);

    return ret;
}

/*
 * The signature similarity between nodes v and u. (1 - the distance between v and u).
 */
double similarity(std::array<int, 73> v, std::array<int, 73> u) {
    double dist = 0;
    for (int i = 0; i < 73; i++) {
        dist += distance(v.at(i), u.at(i), i);
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
    node_degs = (v.at(0) + u.at(0)) / (G_max_deg + H_max_deg);

    return (1 - alpha) * node_degs + alpha * similarity(v, u);
}

/*
 * The maximum degree of all the nodes in the given graph.
 */
int max_deg(std::vector<std::array<int, 73>> gdvs, int graph_order) {
    int max = 0;
    for (int i = 0; i < graph_order; i++) { // gdvs.size()
        if (max < gdvs.at(i).at(0)) {
            max = gdvs.at(i).at(0);
        }
    }

    return max;
}

/*
 * Parse the file at the given path into a heap allocated integer matrix
 * https://www.cplusplus.com/reference/fstream/ifstream/
 * https://www.internalpointers.com/post/beginner-s-look-smart-pointers-modern-c
 */
std::vector<std::array<int, 73>> parse(string filestr) {

    ifstream fin(filestr);
    if (!fin.is_open()) {
        cerr << "Could not open file " << filestr << endl;
        return {};
    }

    // int[][] ret = new int[G_order][]; // change to be one of them smart pointers
    // for (int i = 0; i < G_order; i++) {
    //     ret[i] = new int[73];
    // }

    std::vector<std::array<int, 73>> ret;

    for (int i = 0; i < G_order; i++) {
        std::array<int, 73> gdv;
        for (int j = 0; j < 73; j++) {
            // fin >> ret[i][j];
            fin >> gdv.at(j);
        }
        ret.push_back(gdv);
    }

    fin.close();

    return ret;
}

// /*
//  * Return the number of lines in the given file.
//  */
// int num_lines(string filestr) {

//     ifstream fin(filestr);
//     if (!fin.is_open()) {
//         cerr << "Could not open file " << filestr << endl;
//         return NULL;
//     }

//     // Get the number of lines in the file
//     string line;
//     int lines = 0;

//     while(!fin.eof())
//     {
//         getline(myfile,line);
//         lines++;
//     }

//     fin.close();

//     return lines;
// }

/*
 * Initialize all uninitialized globals.
 */
void init() {
    // Get the number of nodes in G and H
    // G_order = num_lines(G_gdvs_f); 
    // H_order = num_lines(H_gdvs_f);

    // Get the 
    G_gdvs = parse(G_gdvs_f);
    H_gdvs = parse(H_gdvs_f);

    // Get the number of nodes in G and H
    G_order = G_gdvs.size();
    H_order = H_gdvs.size();

    // Calculate the highest degree among all the nodes in G, H
    G_max_deg = max_deg(G_gdvs, G_order);
    H_max_deg = max_deg(G_gdvs, H_order);
}

/*
 * Calculate the topological similarity between the graphs at the given paths.
 */
string top_sim(string gfin, string hfin) {
    // Calculate the graphlet degree vectors for each node of each graph
    G_gdvs_f = orca(gfin);
    //H_gdvs_f = orca(hfin);
    H_gdvs_f = G_gdvs_f;

    init();

    // Calculate the cost matrix for G and H
    double cost_matrix[G_order][H_order];
    for (int i = 0; i < G_order; i++) {
        for (int j = 0; j < H_order; j++) {
            cost_matrix[i][j] = cost(G_gdvs[i], H_gdvs[j]);
        }
    }

    // store the cost_matrix in a file

    return G_gdvs_f; // not this, of courses
}