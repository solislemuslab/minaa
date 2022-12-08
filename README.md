# MiNA: Microbiome Network Alignment Algorithm

## Description

**MiNA** aligns two networks based their topologies and biologies.

## Requirements

This utility requires C++ 20 or higher.

## Compilation

Unix: `make` or `make clean`
Windows: `g++ -O2 -std=c++20 -o mina.exe mina.cpp hungarian.cpp gdvs_dist.cpp graphcrunch.cpp file_io.cpp util.cpp`

## Usage

This utility takes two to seven command-line arguments: `./mina.exe <G> <H> [-B=bio] [-a=alpha] [-b=beta] [-g=gamma] [-merge]`

### Required arguments (ordered)

1. **G**: A graph to align.
2. **H**: A graph to align.

- Require:
  - The graphs are represented by adjacency matrices in CSV format, with labels in  both the first column and row.
  - The CSV delimiter must be one of {comma, semicolon, space, tab}, and will be detected automatically.
  - Any nonzero entry is considered an edge.

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
- **gamma**: the score an aligned pair of nodes must exceed for their alignment to be recorded.
  - Require: a series of real numbers in range [0, 1].
  - Default: 0
- **merge**: A flag indicating to generate a merged visualization of the aligned graphs.

### Outputs

- **X-Y-T/**: (where "X", "Y" are the input graphs, "T" is the date and time of execution) The folder containing the output files specified below.
- **log.txt**: Record of the important details from the alignment.
- **X_gdvs.csv**: (where "X" is the input graph) The Graphlet Degree Vectors for graph "X".
- **top_costs.csv**: The topological cost matrix.
- **bio_costs.csv**: The biologocal cost matrix (as inputed). Not created unless biological input is given.
- **overall_costs.csv**: The combination of the topological and biological cost matrix. Not created unless biological input is given.
- **alignment_list.csv**: A complete list of all aligned nodes, with rows in the format `g_node,h_node,similarity`, descending acording to similarity. The first row in this list is the total cost of the alignment, or the sum of (1 - similarity) for all aligned pairs.
- **alignment_matrix.csv**: A matrix form of the same alignment, where the first column and row are the labels from the two input graphs, respectively.
- **bridged_G.csv**: (where "G" is each gamma specified) The graph bridging the two input graphs, with respect to the alignment.
- **merged_G.csv**: (where "G" is each gamma specified) The graph merging the two input graphs, with respect to the alignment. Not created if the `-merge` flag is not used.

### Examples

`./mina.exe graph0.csv graph1.csv -a=0.6 -g=0,0.7,0.8 -merge`

Here we align graph0 with graph1 using no biological data. `-a=0.6` sets alpha equal to 0.6, meaning 60% of the topological cost function comes from similarity calculated by GDVs, and 40% from simpler node degree data.
`g=0,0.7,0.8` runs the alignment visualization step 3 times: at gamma=0, gamma=0.7, and gamma=0.8. When gamma=0.7, for example, the output will be visualization data for which only aligned pairs of similarity score greater than or equal to 0.7 are actually considered aligned. When gamma=1, only identical nodes will be considered aligned.
The presence of `-merge` indicates that, in addition to the classic alignment visualization for each gamma, we also generate a graph which merges the input graphs intuitively with respect to the alignment.

`./mina.exe graph0.csv graph1.csv bio_costs.csv -b=0.85`

Here we align graph0 with graph1 using topological information and the given biological cost matrix, bio_costs. Since alpha and gamma were unspecified, they default to 0.5 and 1 respectively. Since beta was set to 0.85, 85% of the cost weight is from the topological cost matrix, and 15% is from the given biological cost matrix.

## Visualization Methods (work in progress)

### Bridge

This is what we call the standard method for visualization. Here, the input graphs `G` and `H` are set side by side and an edge is drawn between nodes `g_i` and `h_j` if they are aligned.

### Merge

The merge method of visualization merges the input graphs intuitively, according to the following rules:

- Let network `G` have red nodes `g_i` and edges `g_i,j`, network `H` have blue nodes `h_i` and edges `h_i,j`.
- If `g_i` is aligned to `h_j`, merge these nodes into one node and color it purple.
- If `g_i` is aligned to `h_j`, and `g_k` is aligned to `h_l`, and `g_i` is adjacent to `g_k`, color the edge `g_i,k` red. If instead `h_j` is adjacent to `h_l`, color `h_j,l` blue. If both, color `g_i,k` (aka `h_j,l`) purple.

## Data

No test data is available at this time.
