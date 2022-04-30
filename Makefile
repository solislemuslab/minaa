OBJS	= mna.o top_sim.o orca.o
SOURCE	= mna.cpp top_sim.cpp orca.cpp
HEADER	= top_sim.h orca.h
OUT	    = mna.exe
CC      = g++
FLAGS   = -O2 -g -c -Wall -Wextra -ansi -pedantic

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

mna.o: mna.cpp
	$(CC) $(FLAGS) mna.cpp -std=c++11

top_sim.o: top_sim.cpp
	$(CC) $(FLAGS) top_sim.cpp -std=c++11

orca.o: orca.cpp
	$(CC) $(FLAGS) orca.cpp -std=c++11

clean:
	rm -f $(OBJS) $(OUT) *.out

# Alternate to make
# g++ -O2 -std=c++11 -o mna.exe mna.cpp top_sim.cpp orca.cpp