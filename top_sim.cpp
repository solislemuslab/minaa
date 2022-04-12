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

const int* O = {0, 0};  // Awaiting Milenkovic
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
double distance(int vi, int ui) {
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
        dist += distance(v[i], u[i]);
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
    node_degs = (v[0] + u[0]) / max_deg_G + max_deg_H);

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
    G_order = G[0][0];  // need to figure out how to calculate this now. Could do in parse but that'd be ugly
    H_order = H[0][0];

    // Calculate the highest degree among all the nodes in G, H
    G_max_deg = max_deg(sigs_G, order_G);
    H_max_deg = max_deg(sigs_H, order_H);
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
    for (int i = 0; i < order_G; i++) {
        for (int j = 0; j < order_H; j++) {
            cost_matrix[i][j] = cost(G_gdvs[i], H_gdvs[j]);
        }
    }

    return cost_matrix;
}