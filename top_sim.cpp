// top_sim.cpp
// Topological Similarity Calculator (from GRAAL)
// Reed Nelson

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>

#include "orca.h"

using namespace std;

const double O[73] = {1, 2, 2, 2, 3, 4, 3, 3, 4, 3, 4, 4, 4, 4, 3, 4, 6, 5, 4, 5, 6, 6, 4, 4, 4, 5, 7, 4, 6, 6, 7, 4, 6, 6, 6, 5, 6, 7, 7, 5, 7, 6, 7, 6, 5, 5, 6, 8, 7, 6, 6, 8, 6, 9, 5, 6, 4, 6, 6, 7, 8, 6, 6, 8, 7, 6, 7, 7, 8, 5, 6, 6, 4};  // Awaiting Milenkovic
const double alpha = 0.5;

int64** G_gdvs;  // the GDVs for nodes in G
int64** H_gdvs;  // the GDVs for nodes in H
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
double similarity(int* v, int* u) {
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
double cost(int* v, int* u) {
    double node_degs;
    node_degs = (v[0] + u[0]) / (G_max_deg + H_max_deg);

    return (1 - alpha) * node_degs + alpha * similarity(v, u);
}

/*
 * The maximum degree of all the nodes in the given graph.
 */
int max_deg(int** gdvs, int graph_order) {
    int max = 0;
    for (int i = 0; i < graph_order; i++) {
        if (max < gdvs[i][0]) {
            max = gdvs[i][0];
        }
    }

    return max;
}

/*
 * Parse the file at the given path into a heap allocated integer matrix
 * https://www.cplusplus.com/reference/fstream/ifstream/
 * https://www.internalpointers.com/post/beginner-s-look-smart-pointers-modern-c
 */
int** parse(char* filestr) {

    ifstream fin(filestr);
    if (!fin.is_open()) {
        cerr << "Could not open file " << filestr << endl;
        return NULL;
    }

    int** ret = new int*[G_order];
    for (int i = 0; i < G_order; i++) {
        ret[i] = new int[73];
    }

    for (int i = 0; i < G_order; i++) {
        for (int j = 0; j < 73; j++) {
            fin >> ret[i][j];
        }
    }

    fin.close();

    return ret;
}

/*
 * Initialize all uninitialized globals
 */
void init() {
    // Calculate the number of nodes in G and H
    G_order = size(G_gdvs);  // need to figure out how to calculate this now. Could do in parse but that'd be ugly
    H_order = size(H_gdvs);

    // Calculate the highest degree among all the nodes in G, H
    G_max_deg = max_deg(G_gdvs, G_order);
    H_max_deg = max_deg(G_gdvs, H_order);
}

/*
 *
 */
double** top_sim(char* gfin, char* hfin) {
    // Calculate the graphlet degree vectors for each node of each graph
    G_gdvs = orca(gfin);
    H_gdvs = orca(hfin);

    init();

    // Calculate the cost matrix for G and H
    double** cost_matrix;
    for (int i = 0; i < G_order; i++) {
        for (int j = 0; j < H_order; j++) {
            cost_matrix[i][j] = cost(G_gdvs[i], H_gdvs[j]);
        }
    }

    return cost_matrix;
}