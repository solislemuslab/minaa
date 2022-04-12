# MNA TODO

## Todo

- [ ] Implement the Graphlet Degree Vector function
  - [ ] Configure ORCA to play well with MNA app
    - [x] Throw out major irrelevant bits (1500 lines -> 600 lines!)
    - [x] Convert from comand line executable to an internal function
    - [x] Confirm the modified ORCA works
    - [ ] Implement a means of automatically generating an output file name from the input name
- [ ] Generate the constant O vector needed for the GRAAL score function
- [ ] Implement the GRAAL score function
  - [ ] w_i
  - [ ] D_i(v, u)
  - [ ] D(v, u)
  - [ ] S(v, u)
  - [ ] C(v, u)
- [ ] Implement the Alignment function
- [ ] Glue distinct components together
  - [ ] Decide what file types / graph representations we want where
  - [ ] Implement file converters as necessary
- [ ] Use SPIEC-EASI to generate graphs to run through MNA app

## Done

## Notes

- top_sim is a bad name, probably.
- Clean up in other ways...throw out unnecessary includes...
- Maybe change some char* to strings...
- Change a bunch of things so top_sim looks like proper C++, not C (oops)

Currently, ORCA is passing its heap allocated array of GDV's "int64** orbit" directly to top_sim.
The alternative would be to store it in a file, and then read that file in top_sim.
This alternative would allow us to break more cleanly between ORCA and top_sim,
as we could free all memory allocated by orca before returning to top_sim.
However this would slow down runtime a tiny bit, and the space usage of current method may be negligible.
