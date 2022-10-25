# MNA TODO

## Todo

- [x] Implement the GRAAL cost function (gdvs_dist)
- [ ] Implement the Graphlet Degree Vector function (orca)
  - [ ] Write unit tests
- [x] Implement the alignment algorithm (hungarian)
  - [x] Implement retroactive threshold cutting
- [x] Handle biological input
  - [x] Normalize it to be in range [0,1]
- [x] Write all file IO functions
  - [x] Make alL IO csv
  - [x] Take adj matrix on input
  - [x] Delimiter detection on input
  - [x] Write output to console and log file
  - [x] Generate new folder for each execution
- [ ] Fortify everything with exception handling
- [ ] Use NetCoMi to generate graphs to run through MNA app
- [ ] Verify the correctness/robustness of the complete implementation

## Notes

- There's a bug where, during compilation, a file "graph.out???" is created. I have no idea what that's about.
- ORCA gives incorrect output on 2nd consecutive run.
- Potential problem: in the outputted adjacency matrices, entries of 0 denote there is no edge, but could also denote the cost is 0.

## Questions

- When to do pass by reference?
- What are the implications of using boost? (does it require somehting extra from user, add a lot of overhead...)
