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
#include <string>

#include "top_sim.h"

using namespace std;

/*
 * char* input file 1
 * char* input file 2
 */
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Incorrect number of arguments." << endl;
        cerr << "Usage: mna.exe [graph 1 - input file] [graph 2 - input file]" << endl;
        return 0;
    }

    string gfin = std::string(argv[1]);  // graph G file input
    string hfin = std::string(argv[2]);  // graph H file input

    // Calculate the topological similarity matrix
    top::top_sim(gfin, hfin);

    // Calculate the biological similarity matrix

    // Run the alignment algorithm

    return 0;
}