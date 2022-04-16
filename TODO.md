# MNA TODO

## Todo

- [ ] Implement the GRAAL score function
  - [x] Write w_i, D_i(v, u), D(v, u), S(v, u), C(v, u) into code
  - [ ] Get all data flowing in and out correctly
- [ ] Implement the Alignment function
- [ ] Use SPIEC-EASI to generate graphs to run through MNA app
- [ ] Verify the correctness/robustness of the complete implementation before 
- [ ] Implement a biological score function

## Done

- [x] Implement the Graphlet Degree Vector function
  - [x] Configure ORCA to play well with MNA app
    - [x] Throw out all irrelevant code (1500 lines -> 600 lines!)
    - [x] Convert from comand line executable to an internal function
    - [x] Implement a means of automatically generating an output file name from the input name
- [x] Obtain the constant O vector needed for the GRAAL score function

## Notes

- top_sim is a bad name, probably.
- Clean up in other ways...throw out unnecessary includes...
- Change a bunch of things so top_sim looks like proper C++, not C (oops).
- Figure out the cleanest means of calculating the order of the input networks.
- There's a bug with output file creation