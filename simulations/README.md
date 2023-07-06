# MiNAA: Microbiome Network Alignment Algorithm

Reproducible scripts for the analyses in the manuscript.

## Data

### Generate Synthetic Networks: `generate.R`

This script simulates networks over different numbers of nodes (10, 30, 50, 100, 250 and 500), and different proportions of edges changed (5, 10, and 90 percent). For each of these combinations, 30 networks are generated. Altogether that makes 1,080 simulated networks. The result is stored in the folder *networks/*.
> Warning: the output from this script is 116MB.

### Align Networks in Batch: `align.sh`, `align.bat`

This script sequentially aligns all network pairs output by generate.R. The result is stored in the folder *alignments/*.
> Warning: the output from this script is 318MB.

### Produce Heat Maps: `heatmap.py`

This script generates a heat map for each (graph order, edge flip) combination. The result is a graphic called *heatmap.png*.

### Installation

1. **generate** requirements
   1. Install R
   2. Install DevTools: `install.packages("devtools")`; then load it: `library(devtools)`
   3. Install SpiecEasi:
      1. Download the latest release of [SpiecEasi](https://github.com/zdk123/SpiecEasi/releases)
      2. In R Studio, from the *Install Packages* window, select *Package Archive File* and complete the install
2. **align** requirements
   1. Install C++20
   2. Compile MiNAA
3. **heatmap** requirements
   1. Install Python & Pip
   2. Install dependencies: `pip install numpy pandas matplotlib seaborn`

### Execution

0. `cd simulations`
1. `Rscript generate.R`
2. `./align.sh` (Unix) **OR** `align.bat` (Windows)
3. `python heatmap.py`
