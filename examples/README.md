# Examples

The purpose of this directory is to provide worked example of MiNAA's inputs, execution, and outputs.

`g.csv` is the adjacency matrix for the network G.
`h.csv` is the adjacency matrix for the network H.
`bio.csv` is the biological similarity matrix .

Once MiNAA is successfully compiled, the examples below can be run from this project's root directory. See the main README for compilation steps.

## Example 1

```bash
./minaa.exe examples/g.csv examples/h.csv -a=0.6 -g
```

Output to: `g-h-a0.6/`

Here we align network **g** with network **h** using no biological data. `-a=0.6` sets alpha equal to 0.6, meaning 60% of the topological cost function comes from similarity calculated by GDVs, and 40% from simpler node degree data.

## Example 2

```bash
./minaa.exe examples/g.csv examples/h.csv -B=examples/bio.csv -b=0.85 -st=0.5 -s
```

Output to: `g-h/`

Here we align network **g** with network **h** using topological information and the given biological similarity matrix, **bio**. Since we've provided a similarity matrix instead of a cost matrix (the default), we have to flag that with `-s`. Since alpha was unspecified, it defaults to 1. Since beta was set to 0.85, 85% of the cost weight is from the calculated topological cost matrix, and 15% is from **bio**. Since the similarity threshold `-st=` was set to 0.5, any aligned pair with similarity score less than or equal to 0.5 is excluded from the alignment results.

## Example 3

```bash
./minaa.exe examples/g.csv examples/h.csv -Galias=nonsmoker -Halias=smoker -p -t -c
```

Output to: `nonsmoker-smoker-2024_01_16-22_05_34/`

Here we align network **g** with network **h**, where **g** is given the alias "nonsmoker", and **h** is given the alias "smoker". The timestamp option `-t` was specified, so the name of the output folder will be nonsmoker-smoker-T, where T is the date and time of execution. Because the passthrough option `-p` was specified, g.csv and h.csv will be passed through to the output folder as nonsmoker.csv and smoker.csv, respectively. Finally, because the `-c` option was specified, the output folder will include the alignment's conserved subgraphs, in a file called `conserved_subgraphs.csv`.

## Example 4

```R
source("plot_alignment.R")
plot_alignment(
    g_filepath = "examples/g.csv",
    h_filepath = "examples/h.csv",
    alignment_filepath = "examples/g-h/alignment_matrix.csv",
    output_filepath = "examples/g-h/plot.png",
    st = 0.5,
    hide_singletons = TRUE
)
```

This is an example execution in R of the alignment visualization script `plot_alignment.R`. The resulting `plot.png` is in `examples/g-h/`.

## Attributions

`g.csv`, `h.csv`, and `bio.csv` were produced using data from the paper cited below, which is available [here](https://github.com/lichen-lab/SICS/tree/master/data).

```txt
Xiao, Jian, et al. “A phylogeny-regularized sparse regression model for predictive modeling of Microbial Community Data.” 
Frontiers in Microbiology, vol. 9, 19 Dec. 2018, https://doi.org/10.3389/fmicb.2018.03112. 
```
