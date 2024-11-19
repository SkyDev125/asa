# Directories
CURDIR := $(shell pwd)
OUTDIR = $(CURDIR)/out
REPORTDIR = $(CURDIR)/report

# Source Files
ALL_SRC = $(wildcard $(CURDIR)/*.cpp)
TESTSRC = $(CURDIR)/tests.cpp
SRC = $(filter-out $(TESTSRC), $(ALL_SRC))

# Targets
FILENAME = solution
TESTTARGET = $(OUTDIR)/$(FILENAME).gtest
TARGET = $(OUTDIR)/$(FILENAME).out
DEBUGTARGET = $(OUTDIR)/$(FILENAME).debug
INPUT_FILE = $(CURDIR)/$(FILENAME).IN

# Preprocess the source files to remove main function
PREPROCESSED_SRC = $(patsubst $(CURDIR)/%.cpp, $(OUTDIR)/%.preprocessed.cpp, $(SRC))

# Compiler and flags
CC = g++
CCLAGS = -Wall -Wextra -Werror
GTFLAGS = -lgtest -lgtest_main -pthread
GTOUT = --gtest_output=json:gtest.json
VALFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=$(REPORTDIR)/valgrind.log
GPROFFLAGS = gmon.out -b -p -q -A > $(REPORTDIR)/gprof.log
LIZFLAGS = -l cpp -Eduplicate -a 5 -C 10 -L 1000 -a 5 -E NS -t 4 -o $(REPORTDIR)/lizard.html

# Default rule
all: $(TARGET) valgrind gprof lizard

# interactive rule
inter: $(TARGET) inter-valgrind inter-gprof lizard

# Ensure the output directory exists
$(OUTDIR):
	mkdir -p $(OUTDIR)

# Ensure the report directory exists
$(REPORTDIR):
	mkdir -p $(REPORTDIR)

# Preprocess rule
preprocess: $(OUTDIR) $(REPORTDIR) $(PREPROCESSED_SRC)
$(OUTDIR)/%.preprocessed.cpp: $(CURDIR)/%.cpp
	sed '/int main(/,/}/d' $< > $@

# Run tests.
test: preprocess
	$(CC) $(CCLAGS) -o $(TESTTARGET) $(PREPROCESSED_SRC) $(TESTSRC) $(GTFLAGS)
	rm -rf $(OUTDIR)/*.preprocessed.cpp
	cd $(REPORTDIR) && $(TESTTARGET) $(GTOUT)

# Compile and run checkers
$(TARGET): $(OUTDIR)
	$(CC) $(CCLAGS) -o $(TARGET) $(SRC)

# Compile and run debug
$(DEBUGTARGET): $(OUTDIR)
	$(CC) $(CCLAGS) -pg -o $(DEBUGTARGET) $(SRC)

# Run valgrind
valgrind: $(TARGET) $(REPORTDIR)
	valgrind $(VALFLAGS) $(TARGET) < $(INPUT_FILE)

# Run valgrind Interactive
inter-valgrind: $(TARGET) $(REPORTDIR)
	valgrind $(VALFLAGS) $(TARGET)

# Run gprof
gprof: $(DEBUGTARGET) $(REPORTDIR)
	$(DEBUGTARGET) < $(INPUT_FILE)
	gprof $(DEBUGTARGET) $(GPROFFLAGS)
	rm gmon.out

# Run gprof Interactive
inter-gprof: $(DEBUGTARGET) $(REPORTDIR)
	$(DEBUGTARGET)
	gprof $(DEBUGTARGET) $(GPROFFLAGS)
	rm gmon.out

# Run lizard
lizard: $(REPORTDIR)
	lizard $(LIZFLAGS) $(SRC)

# Clean rule
clean:
	rm -rf $(OUTDIR)
	rm -rf $(REPORTDIR)