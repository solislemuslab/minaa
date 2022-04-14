// mna.cpp
// Microbial Network Alignment
// Reed Nelson

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>

#include "top_sim.h"
// #include "orca.h"  // debug

using namespace std;

/*
 * char* input file 1
 * char* input file 2
 * char* output location
 */
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Incorrect number of arguments." << endl;
        cerr << "Usage: mna.exe [graph 1 - input file] [graph 2 - input file]" << endl;
        return 0;
    }

    char* gfin = argv[1];  // graph G file input
    char* hfin = argv[2];  // graph H file input

    // orca(gfin);  // debug (circumvent top_sim)

    // Calculate the topological similarity matrix
    top_sim(gfin, hfin);

    // Calculate the biological similarity matrix

    // Run the alignment algorithm

    return 0;
}