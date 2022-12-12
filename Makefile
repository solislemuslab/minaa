#OBJS	= minaa.o hungarian.o gdvs_dist.o graphcrunch.o file_io.o util.o
SOURCE	= minaa.cpp hungarian.cpp gdvs_dist.cpp graphcrunch.cpp file_io.cpp util.cpp
HEADER	= hungarian.h gdvs_dist.h graphcrunch.h file_io.h util.h
TARGET  = minaa.exe
CC      = g++
FLAGS   = -O3 -g -c -Wall -Wextra -ansi -pedantic -std=c++20 -Iinclude

HEADER_FILES = $(addprefix include/,$(HEADER))
SOURCE_FILES = $(addprefix src/,$(SOURCE))
OBJECT_FILES = $(addprefix obj/,$(SOURCE:.cpp=.o))

all: $(OBJECT_FILES)
	$(CC) -g $(OBJECT_FILES) -o $(TARGET)

obj/%.o: src/%.cpp $(HEADER_FILES)
	@mkdir -p $(@D)
	$(CC) $(FLAGS) -o $@ $<

clean:
	rm -f $(TARGET) obj/*.o

# Alternate to make
# g++ -O3 -std=c++20 -o minaa.exe minaa.cpp hungarian.cpp gdvs_dist.cpp graphcrunch.cpp file_io.cpp util.cpp