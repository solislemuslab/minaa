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

1. G; A network to align.
2. H; A network to align.

- Require:
  - The networks are represented by adjacency matrices in CSV format, with labels in both the first column and row.
  - The CSV delimiter must be one of {comma, semicolon, space, tab}, and will be detected automatically.
  - Any nonzero entry is considered an edge.
  - |G| is lesser or equal to |H|.

### Optional Arguments (unordered)

#### Common

- **-B=**: The path to the biological cost matrix file.
  - Require: CSV adjacency matrix where the first column consists of the labels of G, and first row consists of the labels of H.
  - Default: the algorithm will run using only topological calculations.
- **-a=**: alpha; the GDV-edge weight balancer.
  - Require: a real number in range [0, 1].
  - Default: 1
- **-b=**: beta; the topological-biological cost matrix balancer.
  - Require: a real number in range [0, 1].
  - Default: 1

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
- **-t**: timestamp; the output folder's name includes the date and time of execution.
  - Require: none.
  - Default: the output folder's name does not include date and time.
- **-g**: greekstamp; the output folder's name includes the values for alpha and beta.
  - Require: none.
  - Default: the output folder's name does not include the values for alpha and beta.

### Outputs

- **G-H/**: (where G, H are the input networks) The folder containing the output files specified below.
- **log.txt**: record of the important details from the alignment.
- **G_gdvs.csv**: (where G is the input network) the Graphlet Degree Vectors for network G.
- **H_gdvs.csv**: (where H is the input network) the Graphlet Degree Vectors for network H.
- **top_costs.csv**: the topological cost matrix.
- **bio_costs.csv**: the biologocal cost matrix (as inputed). Not created unless biological input is given.
- **overall_costs.csv**: the combination of the topological and biological cost matrix. Not created unless biological input is given.
- **alignment_list.csv**: a complete list of all aligned nodes, with rows in the format `g_node,h_node,similarity`, descending acording to similarity. The first row in this list is the total cost of the alignment, or the sum of (1 - similarity) for all aligned pairs.
- **alignment_matrix.csv**: a matrix form of the same alignment, where the first column and row are the labels from the two input networks, respectively.

### Examples

`./minaa.exe network0.csv network1.csv -a=0.6`

Here we align network0 with network1 using no biological data. `-a=0.6` sets alpha equal to 0.6, meaning 60% of the topological cost function comes from similarity calculated by GDVs, and 40% from simpler node degree data.

`./minaa.exe network0.csv network1.csv -B=bio_costs.csv -b=0.85`

Here we align network0 with network1 using topological information and the given biological cost matrix, bio_costs. Since alpha was unspecified, it defaults to 1. Since beta was set to 0.85, 85% of the cost weight is from the topological cost matrix, and 15% is from the given biological cost matrix.

`./minaa.exe network0.csv network1.csv -Galias=control -Halias=treatment -p -t`

Here we align network0 with network1, where network1 is given the alias "control", and network2 is given the alias "treatment". The timestamp option was specified, so the name of the output folder will be control-treatment-T, where T is the date and time of execution. Additionally, because the passthrough option was specified, network0.csv and network1.csv will be passed through to the output folder as control.csv and treatment.csv, respectively.

See the `example/` directory for a sample input and output to MiNAA, which you can look at and replicate yourself.

## Simulations in the Manuscript

All scripts and instructions to reproduce the analyses in the manuscript can be found in the `simulations` folder.

## Contributions, Questions, Issues, and Feedback

Users interested in expanding functionalities in MiNAA are welcome to do so. Issues reports are encouraged through Github's [issue tracker](https://github.com/solislemuslab/minaa/issues). See details on how to contribute and report issues in [CONTRIBUTING.md](https://github.com/solislemuslab/minaa/blob/master/CONTRIBUTING.md).

## License

MiNAA is licensed under the [MIT](https://opensource.org/licenses/MIT) licence. &copy; SolisLemus lab (2023).

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
