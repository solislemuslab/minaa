# Microbiome Network Alignment

## Description

The **Microbiome Network Alignment** algorithm aligns two networks based their topologies and biologies.

## Compilation

Compile with `make` or `g++ -O2 -std=c++20 -o mna.exe mna.cpp hungarian.cpp gdvs_dist.cpp graphcrunch.cpp file_io.cpp util.cpp`

## Usage

This utility takes two to seven command-line arguments: `./mna.exe <G> <H> [-B=bio] [-a=alpha] [-b=beta] [-g=gamma] [-merge]`

### Required arguments (ordered)

1. **G**: A graph to align
   - Require: the graph is represented by an adjacency list or matrix in CSV format
2. **H**: A graph to align
   - Require: the graph is represented by an adjacency list or matrix in CSV format

### Optional arguments (unordered)

- **bio**: The path to the biological cost matrix file
  - Require: CSV file type
  - Default: the algorithm will run using only topological calculations
- **alpha**: GDV-edge weight balancer
  - Require: a real number in range [0, 1]
  - Default: 1
- **beta**: topological-biological cost matrix balancer
  - Require: a real number in range [0, 1]
  - Default: 1
- **gamma**: the score an aligned pair of nodes must exceed for their alignment to be recorded
  - Require: a series of real numbers in range [0, 1]
  - Default: 0
- **merge**: A flag indicating to generate a merged visualization of the aligned graphs

### Examples

`./mna.exe graph0.csv graph1.csv -a=0.6 -g=0,0.7,0.8 -merge`

Here we align graph0 with graph1 using no biological data. `-a=0.6` sets alpha equal to 0.6, meaning 60% of the topological cost function comes from similarity calculated by GDVs, and 40% from simpler node degree data.
`g=0,0.7,0.8` runs the alignment visualization step 3 times: at gamma=0, gamma=0.7, and gamma=0.8. When gamma=0.7, for example, the output will be visualization data for which only aligned pairs of similarity score greater than or equal to 0.7 are actually considered aligned. When gamma=1, only identical nodes will be considered aligned.
The presence of `-merge` indicates that, in addition to the classic alignment visualization for each gamma, we also generate a graph which merges the input graphs intuitively with respect to the alignment.

`./mna.exe graph0.csv graph1.csv bio_costs.csv -b=0.85`

Here we align graph0 with graph1 using topological information and the given biological cost matrix, bio_costs. Since alpha and gamma were unspecified, they default to 0.5 and 1 respectively. Since beta was set to 0.85, 85% of the cost weight is from the topological cost matrix, and 15% is from the given biological cost matrix.

<!-- ### Visualization Methods

#### Bridge

This is the 

#### Merge

The merge method of visualization merges the input graphs intuitively, formalized by the following rules:

- Let network G have red nodes g_i and edges g_i,j, network H have blue nodes h_i and edges h_i,j.
- Take '=' to mean 'is aligned to'
- If g_i = h_j, merge these nodes into one, and color it purple.
- If g_i = h_j, and g_k = h_l, 

1. merged_i,j = 0 iff there is no edge between nodes i and j
2. merged_i,j = 1 iff only G draws an edge between nodes i and j
3. merged_i,j = 2 iff only H draws an edge between nodes i and j
4. merged_i,j = 3 iff both G and H draw an edge between nodes i and j -->

## Data

No test data is available at this time.
