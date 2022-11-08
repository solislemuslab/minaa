OBJS	= mna.o hungarian.o gdvs_dist.o graphcrunch.o file_io.o util.o
SOURCE	= mna.cpp hungarian.cpp gdvs_dist.cpp graphcrunch.cpp file_io.cpp util.cpp
HEADER	= hungarian.h gdvs_dist.h graphcrunch.h file_io.h util.h
OUT	    = mna.exe
CC      = g++
FLAGS   = -O2 -g -c -Wall -Wextra -ansi -pedantic

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

mna.o: mna.cpp
	$(CC) $(FLAGS) mna.cpp -std=c++20

hungarian.o: hungarian.cpp
	$(CC) $(FLAGS) hungarian.cpp -std=c++20

gdvs_dist.o: gdvs_dist.cpp
	$(CC) $(FLAGS) gdvs_dist.cpp -std=c++20

graphcrunch.o: graphcrunch.cpp
	$(CC) $(FLAGS) graphcrunch.cpp -std=c++20

file_io.o: file_io.cpp
	$(CC) $(FLAGS) file_io.cpp -std=c++20

util.o: util.cpp
	$(CC) $(FLAGS) util.cpp -std=c++20

clean:
	$(CC) -g $(OBJS) -o $(OUT)
	rm -f $(OBJS)

# Alternate to make
# g++ -O2 -std=c++20 -o mna.exe mna.cpp hungarian.cpp gdvs_dist.cpp graphcrunch.cpp file_io.cpp util.cpp