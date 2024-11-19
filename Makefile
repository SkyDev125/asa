# Directories
CURDIR := $(shell pwd)
OUTDIR = $(CURDIR)/out

# Source Files
ALL_SRC = $(wildcard $(CURDIR)/*.cpp)
TESTSRC = $(CURDIR)/tests.cpp
SRC = $(filter-out $(TESTSRC), $(ALL_SRC))

# Targets
FILENAME = solution
TESTTARGET = $(OUTDIR)/$(FILENAME).test
TARGET = $(OUTDIR)/$(FILENAME).out

# Preprocess the source files to remove main function
PREPROCESSED_SRC = $(patsubst $(CURDIR)/%.cpp, $(OUTDIR)/%.preprocessed.cpp, $(SRC))

# Compiler and flags
CC = g++
CCLAGS = -Wall -pg
GTFLAGS = -lgtest -lgtest_main -pthread
GTOUT = --gtest_output=json:gtest.json
VALFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=$(OUTDIR)/valgrind.log
GPROFFLAGS = gmon.out -b -p -q -A > $(OUTDIR)/gprof.log
LIZFLAGS = -l cpp -Eduplicate -a 5 -C 10 -L 1000 -a 5 -E NS -t 4 -o $(OUTDIR)/lizard.html

# Default rule
all: $(TARGET) valgrind gprof lizard

# Ensure the output directory exists
$(OUTDIR):
	mkdir -p $(OUTDIR)

# Preprocess rule
preprocess: $(OUTDIR) $(PREPROCESSED_SRC)
$(OUTDIR)/%.preprocessed.cpp: $(CURDIR)/%.cpp
	sed '/int main(/,/}/d' $< > $@

# Run tests.
test: preprocess
	$(CC) $(CCLAGS) -o $(TESTTARGET) $(PREPROCESSED_SRC) $(TESTSRC) $(GTFLAGS)
	rm -rf $(OUTDIR)/*.preprocessed.cpp
	cd $(OUTDIR) && $(TESTTARGET) $(GTOUT)

# Compile and run checkers
$(TARGET): $(OUTDIR)
	$(CC) $(CCLAGS) -o $(TARGET) $(SRC)


# Run valgrind
valgrind: $(TARGET)
	valgrind $(VALFLAGS) $(TARGET)

# Run gprof
gprof: $(TARGET)
	gprof $(TARGET) $(GPROFFLAGS)
	rm gmon.out

# Run lizard
lizard:
	lizard $(LIZFLAGS) $(SRC)

# Clean rule
clean:
	rm -rf $(OUTDIR)