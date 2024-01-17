# MiNAA: Microbiome Network Alignment Algorithm

<img src="logo.png" style="width:40%;" align=right>

[![GitHub Releases](https://img.shields.io/github/v/release/solislemuslab/minaa?display_name=tag)](https://github.com/solislemuslab/minaa/releases) [![GitHub license](https://img.shields.io/github/license/solislemuslab/minaa)](https://github.com/solislemuslab/minaa/blob/main/LICENCE) [![GitHub Issues](https://img.shields.io/github/issues/solislemuslab/minaa)](https://github.com/solislemuslab/minaa/issues) ![ ](https://img.shields.io/github/languages/code-size/solislemuslab/minaa) [![status](https://joss.theoj.org/papers/b4d9f26021065b1759d50413f60aa9c3/status.svg)](https://joss.theoj.org/papers/b4d9f26021065b1759d50413f60aa9c3)

## Description

MiNAA takes as input a pair of node-edge networks, and finds a correspondance between them such that each node in one is mapped to its most similar node in the other. MiNAA is capable of using both *topological* (structural) information about the network, and *biological* information about the taxa each node represents, in order to produce a good approximation of the optimal alignment. Due to the complexity of this task, an approximation is the best that can be done in an efficient runtime. Network alignment in this setting is done primarily for comparative purposes. For example, an alignment might map clusters of taxa to each other, revealing conserved or analogous functions between microbial communities. See our [software note](https://arxiv.org/abs/2212.05880) (preprint) for additional details.

## Requirements

This program requires C++20 or higher, and g++.

## Compilation

### Unix

```bash
make
```

### Windows

```bash
mkdir obj
make
```

In addition to C++20 and g++, Windows requires a special means to run the provided makefile. The MinGW Package Manager provides a lightweight make function. It is recommended to download MinGW [here](https://sourceforge.net/projects/mingw/), and follow [this guide](https://linuxhint.com/run-makefile-windows/) for installation, however any method for compiling C++ using g++ should suffice.

## Usage

This utility has the form `./minaa.exe <G> <H> [-B=bio] [-a=alpha] [-b=beta]`.

### Required Arguments (ordered)

1. G; a network to align.
2. H; a network to align.

- Require:
  - The networks are represented by adjacency matrices in CSV format, with labels in both the first column and row.
  - The CSV delimiter must be one of {comma, semicolon, space, tab}, and will be detected automatically.
  - |G| is lesser or equal to |H|.
- Notes:
  - Any nonzero entry is considered an edge.

### Optional Arguments (unordered)

#### Common

- **-B=**: the path to the biological cost matrix file.
  - Require: CSV adjacency matrix where the first column consists of the labels of G, in order, and first row consists of the labels of H, in order.
  - Default: the algorithm will run using only topological calculations.
  - Notes:
    - The input matrix is normalized by MiNAA such that all entries are in range [0, 1].
    - The input is assumed to be a cost matrix. If it is a similarity matrix, use the **-s** option detailed below.
- **-a=**: alpha; the GDV-edge weight balancer.
  - Require: a real number in range [0, 1].
  - Default: 1 (100% GDV data).
- **-b=**: beta; the topological-biological cost matrix balancer.
  - Require: a real number in range [0, 1].
  - Default: 1 (100% topological data).
- **-st=**: similarity threshold; The similarity value above which aligned pairs are included in the output.
  - Require: a real number in range [0, 1].
  - Default: 0.

#### Uncommon

- **-Galias=**: an alias for the G file.
  - Require: a valid file name.
  - Default: the G file keeps its original name.
- **-Halias=**: an alias for the H file.
  - Require: a valid file name.
  - Default: the H file keeps its original name.
- **-Balias=**: an alias for the B file.
  - Require: a valid file name.
  - Default: the B file keeps its original name.
- **-p**: passthrough; whether or not to write the input files into the output folder.
  - Require: none.
  - Default: the files are not passed through to the output folder.
  - Note: The output reflects the input data after having been processed by the algorithm, this is not a direct copy and paste.
- **-t**: timestamp; the output folder's name includes the date and time of execution.
  - Require: none.
  - Default: the output folder's name does not include date and time.
- **-g**: greekstamp; the output folder's name includes the values for alpha and beta.
  - Require: none.
  - Default: the output folder's name does not include the values for alpha and beta.
- **-s**: similarity conversion; for each entry in the given biological matrix, the value (post normalization) is replaced with 1 - value. Use this if and only if the provided biological matrix is a similarity matrix.
  - Require: none.
  - Default: the given biological matrix is left as is.

### Outputs

- **G-H/**: (where G, H are the input networks) The folder containing the output files specified below.
- **log.txt**: record of the important details from the alignment.
- **G_gdvs.csv**: (where G is the input network) the Graphlet Degree Vectors for network G.
- **H_gdvs.csv**: (where H is the input network) the Graphlet Degree Vectors for network H.
- **top_costs.csv**: the topological cost matrix.
- **bio_costs.csv**: the biologocal cost matrix (as inputed). Not created unless biological input is given.
- **overall_costs.csv**: the combination of the topological and biological cost matrix. Not created unless biological input is given.
- **alignment_list.csv**: a complete list of all aligned nodes, with rows in the format `g_node,h_node,similarity`, descending acording to similarity. The first row in this list is the total *cost* of the alignment, or the sum of (1 - similarity) for all aligned pairs.
- **alignment_matrix.csv**: a matrix form of the same alignment, where the first column and row are the labels from the two input networks, respectively.

### Examples

Examples of MiNAA's usage with real data and in-depth explanations can be found in the `examples/` directory.

## Simulations in the Manuscript

All scripts and instructions to reproduce the analyses in the manuscript can be found in the `simulations/` directory.

## Contributions, Questions, Issues, and Feedback

Users interested in expanding functionalities in MiNAA are welcome to do so. Issues reports are encouraged through Github's [issue tracker](https://github.com/solislemuslab/minaa/issues). See details on how to contribute and report issues in [CONTRIBUTING.md](https://github.com/solislemuslab/minaa/blob/master/CONTRIBUTING.md).

## License

MiNAA is licensed under the [MIT](https://opensource.org/licenses/MIT) licence. &copy; SolisLemus lab (2024).

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
