SOURCE	= minaa.cpp hungarian.cpp gdvs_dist.cpp graphcrunch.cpp file_io.cpp util.cpp
HEADER	= hungarian.h gdvs_dist.h graphcrunch.h file_io.h util.h
TARGET  = minaa.exe
CC      = g++
FLAGS   = -O3 -g -c -Wall -Wextra -ansi -pedantic -std=c++20 -Iinclude

HEADER_FILES = $(addprefix include/,$(HEADER))
SOURCE_FILES = $(addprefix src/,$(SOURCE))
OBJECT_FILES = $(addprefix obj/,$(SOURCE:.cpp=.o))

ifdef SystemRoot # Windows
    RM = del /Q
	RMOBJ = obj\*.o
	MKDIR =
else 			 # Unix
	RM = rm -f
	RMOBJ = obj/*.o
	MKDIR = @mkdir -p $(@D)
endif

all: $(OBJECT_FILES)
	$(CC) -g $(OBJECT_FILES) -o $(TARGET)

obj/%.o: src/%.cpp $(HEADER_FILES)
	$(MKDIR)
	$(CC) $(FLAGS) -o $@ $<

clean:
	$(RM) $(TARGET) $(RMOBJ)
