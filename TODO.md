# MNA TODO

## Todo

- [x] Implement the GRAAL cost function (gdvs_dist)
- [ ] Implement the Graphlet Degree Vector function (orca)
  - [ ] Write unit tests
- [x] Implement the alignment algorithm (hungarian)
  - [ ] Implement retroactive threshold cutting
- [ ] Modify the alignment algorithm for the partial case?
- [x] Handle biological input
  - [x] Normalize it to be in range [0,1]
- [ ] Write all file IO functions
  - [ ] Make everything proper csv
  - [ ] On input: allow matrix or list
  - [ ] On input: delimiter detection
- [ ] Fortify everything with exception handling
- [ ] Use NetCoMi to generate graphs to run through MNA app
- [ ] Verify the correctness/robustness of the complete implementation

## Notes

- There's a bug where, during compilation, a file "graph.out???" is created. I have no idea what that's about.
- ORCA gives incorrect output on 2nd consecutive run.

## Questions for Arnaud

- When to do pass by reference?
