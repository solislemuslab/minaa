# MiNAA Example

The purpose of this folder is to provide a basic worked example of MiNAA's inputs, execution, and outputs.

`G.csv` is the adjacency matrix for the network G.
`H.csv` is the adjacency matrix for the network H.
`G-H/` is the folder containing the output files from the exection below.

`G.csv` and `H.csv` are networks of 250 nodes, generated using the `generate.R` script in the `simulations` folder. H is a mutation of G in which 10% of the possible or actual edges were flipped from their state in G to the opposite state.

## Unix Example

From the project root directory run:

```bash
make
./minaa.exe example/G.csv example/H.csv
```
