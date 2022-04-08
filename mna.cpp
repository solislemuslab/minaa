#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "orca.h"

using namespace std;

/*
 * char* input file 1
 * char* input file 2
 * char* output location
 */
int main(int argc, char* argv[])
{
    if (argc != 4) {
		cerr << "Incorrect number of arguments." << endl;
		cerr << "Usage: mna.exe [graph 1 - input file] [graph 2 - input file] [alignment - output file]" << endl;
		return 0;
	}
 
	char* afin = argv[1];
	char* bfin = argv[2];
	char* fout = argv[3]; 

	char* afout = "orbit-counts.out"; 
	char* bfout = "orbit-counts.out"; 

	orca(afin, afout);
 
    return 0;
}