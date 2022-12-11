# MiNAA: Microbiome Network Alignment Algorithm

## Description

**MiNAA** aligns two networks based their topologies and biologies.

## Requirements

This program requires C++20 or higher.

## Compilation

Unix: `make` or `make clean`

Windows: `g++ -O3 -std=c++20 -o minaa.exe minaa.cpp hungarian.cpp gdvs_dist.cpp graphcrunch.cpp file_io.cpp util.cpp`

## Usage

This utility takes two to five command-line arguments: `./minaa.exe <G> <H> [-B=bio] [-a=alpha] [-b=beta]`

### Required arguments (ordered)

1. **G**: A network to align.
2. **H**: A network to align.

- Require:
  - The networks are represented by adjacency matrices in CSV format, with labels in both the first column and row.
  - The CSV delimiter must be one of {comma, semicolon, space, tab}, and will be detected automatically.
  - Any nonzero entry is considered an edge.
  - |G| should be lesser or equal to |H|.

### Optional arguments (unordered)

- **bio**: The path to the biological cost matrix file.
  - Require: CSV adjacency matrix where the first column is the labels of G, and first row is the labels of H.
  - Default: the algorithm will run using only topological calculations.
- **alpha**: GDV-edge weight balancer
  - Require: a real number in range [0, 1].
  - Default: 1
- **beta**: topological-biological cost matrix balancer.
  - Require: a real number in range [0, 1].
  - Default: 1

### Outputs

- **X-Y-T/**: (where "X", "Y" are the input networks, "T" is the date and time of execution) The folder containing the output files specified below.
- **log.txt**: Record of the important details from the alignment.
- **X_gdvs.csv**: (where "X" is the input network) The Graphlet Degree Vectors for network "X".
- **top_costs.csv**: The topological cost matrix.
- **bio_costs.csv**: The biologocal cost matrix (as inputed). Not created unless biological input is given.
- **overall_costs.csv**: The combination of the topological and biological cost matrix. Not created unless biological input is given.
- **alignment_list.csv**: A complete list of all aligned nodes, with rows in the format `g_node,h_node,similarity`, descending acording to similarity. The first row in this list is the total cost of the alignment, or the sum of (1 - similarity) for all aligned pairs.
- **alignment_matrix.csv**: A matrix form of the same alignment, where the first column and row are the labels from the two input networks, respectively.

### Examples

`./minaa.exe network0.csv network1.csv -a=0.6`

Here we align network0 with network1 using no biological data. `-a=0.6` sets alpha equal to 0.6, meaning 60% of the topological cost function comes from similarity calculated by GDVs, and 40% from simpler node degree data.

`./minaa.exe network0.csv network1.csv bio_costs.csv -b=0.85`

Here we align network0 with network1 using topological information and the given biological cost matrix, bio_costs. Since alpha and gamma were unspecified, they default to 0.5 and 1 respectively. Since beta was set to 0.85, 85% of the cost weight is from the topological cost matrix, and 15% is from the given biological cost matrix.

## Data

### Generate Synthetic Networks

*simulate/generate.R*

This script simulates networks over different numbers of nodes (10, 30, 50, 100, 250 and 500), and different proportions of edges changed (5, 10, and 90 percent). For each of these combinations, 30 networks are generated. Altogether that makes 1,080 simulated networks. The result is stored in the folder *networks/*.
> Warning: the output from this script is 116MB.

### Align Networks in Batch

Unix: *simulate/align.sh*
Windows: *simulate/align.bat*

This script sequentially aligns all network pairs output by generate.R. The result is stored in the folder *outputs/*.
> Warning: the output from this script is 318MB.

### Produce Heat Maps

*simulate/heatmap.py*

This script generates a heat map for each (graph order, edge flip) combination. The result is a graphic called *heatmap.png*.

### Installation

1. **generate** requirements
   1. Install R
   2. Install DevTools: `install.packages("devtools")`; then load it: `library(devtools)`
   3. Install SpiecEasi: `install_github("zdk123/SpiecEasi")`
2. **align** requirements
   1. Install C++20
   2. Compile MiNAA
3. **heatmap** requirements
   1. Install Python & Pip
   2. Install dependencies: `pip install numpy pandas matplotlib seaborn`

### Execution

0. `cd simulate`
1. `Rscript generate.R`
2. `./align.sh` (Unix) **OR** `align.bat` (Windows)
3. `python heatmap.py`
