# MiNAA: Microbiome Network Alignment Algorithm

[![GitHub Releases](https://img.shields.io/github/v/release/solislemuslab/minaa?display_name=tag)](https://github.com/solislemuslab/minaa/releases) [![GitHub license](https://img.shields.io/github/license/solislemuslab/minaa)](https://github.com/solislemuslab/minaa/blob/main/LICENCE) [![GitHub Issues](https://img.shields.io/github/issues/solislemuslab/minaa)](https://github.com/solislemuslab/minaa/issues) ![ ](https://img.shields.io/github/languages/code-size/solislemuslab/minaa)

## Description

**MiNAA** aligns two networks based their topologies and biologies.

## Requirements

This program requires C++20 or higher.

## Compilation

### Unix

`make`

### Windows

```cmd
mkdir obj
make
```

In addition to C++20, Windows requires a special means to run the provided makefile. The MinGW Package Manager provides a lightweight make function. It is recommended to follow [this guide](https://linuxhint.com/run-makefile-windows/), however any method for compiling C++ should suffice.

## Usage

This utility takes two to five command-line arguments: `./minaa.exe <G> <H> [-B=bio] [-a=alpha] [-b=beta]`

### Required Arguments (ordered)

1. **G**: A network to align.
2. **H**: A network to align.

- Require:
  - The networks are represented by adjacency matrices in CSV format, with labels in both the first column and row.
  - The CSV delimiter must be one of {comma, semicolon, space, tab}, and will be detected automatically.
  - Any nonzero entry is considered an edge.
  - |G| should be lesser or equal to |H|.

### Optional Arguments (unordered)

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

- **X-Y-T/**: (where "X", "Y" are the input networks, "T" is the date and time of execution) The folder containing the output files specified below.
- **log.txt**: Record of the important details from the alignment.
- **X_gdvs.csv**: (where "X" is the input network) The Graphlet Degree Vectors for network "X".
- **top_costs.csv**: The topological cost matrix.
- **bio_costs.csv**: The biologocal cost matrix (as inputed). Not created unless biological input is given.
- **overall_costs.csv**: The combination of the topological and biological cost matrix. Not created unless biological input is given.
- **alignment_list.csv**: A complete list of all aligned nodes, with rows in the format `g_node,h_node,similarity`, descending acording to similarity. The first row in this list is the total cost of the alignment, or the sum of (1 - similarity) for all aligned pairs.
- **alignment_matrix.csv**: A matrix form of the same alignment, where the first column and row are the labels from the two input networks, respectively.
- **bridged_G.csv**: (where "G" is each gamma specified) The graph bridging the two input graphs, with respect to the alignment.
- **merged_G.csv**: (where "G" is each gamma specified) The graph merging the two input graphs, with respect to the alignment. Not created if the `-merge` flag is not used.

### Examples

`./minaa.exe graph0.csv graph1.csv -a=0.6 -g=0,0.7,0.8 -merge`

Here we align graph0 with graph1 using no biological data. `-a=0.6` sets alpha equal to 0.6, meaning 60% of the topological cost function comes from similarity calculated by GDVs, and 40% from simpler node degree data.
`g=0,0.7,0.8` runs the alignment visualization step 3 times: at gamma=0, gamma=0.7, and gamma=0.8. When gamma=0.7, for example, the output will be visualization data for which only aligned pairs of similarity score greater than or equal to 0.7 are actually considered aligned. When gamma=1, only identical nodes will be considered aligned.
The presence of `-merge` indicates that, in addition to the classic alignment visualization for each gamma, we also generate a graph which merges the input graphs intuitively with respect to the alignment.

`./minaa.exe network0.csv network1.csv bio_costs.csv -b=0.85`

Here we align network0 with network1 using topological information and the given biological cost matrix, bio_costs. Since alpha and gamma were unspecified, they default to 0.5 and 1 respectively. Since beta was set to 0.85, 85% of the cost weight is from the topological cost matrix, and 15% is from the given biological cost matrix.

## Visualization Methods

> Note: this feature is a work-in-progress, and does not yield graphics at this time.

### Bridge

This is what we call the standard method for visualization. Here, the input graphs `G` and `H` are set side by side and an edge is drawn between nodes `g_i` and `h_j` if they are aligned.

### Merge

The merge method of visualization merges the input graphs intuitively, according to the following rules:

- Let network `G` have red nodes `g_i` and edges `g_i,j`, network `H` have blue nodes `h_i` and edges `h_i,j`.
- If `g_i` is aligned to `h_j`, merge these nodes into one node and color it purple.
- If `g_i` is aligned to `h_j`, and `g_k` is aligned to `h_l`, and `g_i` is adjacent to `g_k`, color the edge `g_i,k` red. If instead `h_j` is adjacent to `h_l`, color `h_j,l` blue. If both, color `g_i,k` (aka `h_j,l`) purple.

## Simulations in the Manuscript

All scripts and instructions to reproduce the analyses in the manuscript can be found in the `simulations` folder.

## Contributions, Questions, Issues, and Feedback

Users interested in expanding functionalities in MiNAA are welcome to do so. Issues reports are encouraged through Github's [issue tracker](https://github.com/solislemuslab/minaa/issues). See details on how to contribute and report issues in [CONTRIBUTING.md](https://github.com/solislemuslab/minaa/blob/master/CONTRIBUTING.md).

## License

MiNAA is licensed under the [MIT](https://opensource.org/licenses/MIT) licence. &copy; SolisLemus lab projects (2022)

## Citation

If you use MiNAA in your work, we kindly ask that you cite the following paper:

```bibtex
@ARTICLE{Nelson2022,
  title         = "MiNAA: Microbiome Network Alignment Algorithm",
  author        = "Nelson, Reed and Aghdam, Rosa and
                   Solis-Lemus, Claudia",
  year          =  2022,
  archivePrefix = "arXiv",
  primaryClass  = "q-bio.PE",
  eprint        = "xxx"
}
```
