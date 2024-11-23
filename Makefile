# Directories
CURDIR := $(shell pwd)
OUTDIR = $(CURDIR)/out
REPORTDIR = $(CURDIR)/report
TESTDIR = $(CURDIR)/tests

# Source Files
SRC = $(wildcard $(CURDIR)/*.cpp)
TEST_INPUTS = $(wildcard $(TESTDIR)/*.in)
TEST_OUTPUTS = $(wildcard $(TESTDIR)/*.out)
TOTAL_ERRORS = 

# Targets
FILENAME = proj
TARGET = $(OUTDIR)/$(FILENAME).out
DEBUGTARGET = $(OUTDIR)/$(FILENAME).debug
INPUT_FILE = $(CURDIR)/$(FILENAME).in
TEST_OUT_EXT = .outme

# Compiler and flags
CC = g++
CCLAGS = -std=c++11 -O3 -Wall
VALFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=$(REPORTDIR)/valgrind.log
GPROFFLAGS = gmon.out -b -p -q -A > $(REPORTDIR)/gprof.log
LIZFLAGS = -l cpp -Eduplicate -a 5 -C 10 -L 1000 -a 5 -E NS -t 4 -o $(REPORTDIR)/lizard.html
HYPERFLAGS = -N --command-name "Benchmark 1" --runs 100 2>/dev/null

# Default rule
all: $(TARGET) valgrind gprof lizard

# interactive rule
inter: $(TARGET) inter-valgrind inter-gprof lizard

# Test rule
test: clean $(TESTDIR) $(TEST_INPUTS) $(TEST_OUTPUTS) $(TARGET)
	@TOTAL_ERRORS=""; \
	TOTAL_PASSES=""; \
	FAILED_COUNT=0; \
	PASSED_COUNT=0; \
	echo "<================= Running Tests ==================>"; \
	for file in $(TEST_INPUTS); do \
		RELATIVE_FILE=$$(realpath --relative-to=. $${file%.in}$(TEST_OUT_EXT)); \
		$(TARGET) < $$file > $${file%.in}.outme; \
		TMP_DIFF=$$(mktemp); \
		if ! diff $${file%.in}.outme $${file%.in}.out > $$TMP_DIFF; then \
			mv $$TMP_DIFF $${file%.in}.diff; \
			echo "[FAIL] $$(basename $${file%.in})"; \
			if [ $$FAILED_COUNT -gt 0 ]; then \
				TOTAL_ERRORS="$${TOTAL_ERRORS}\n$$RELATIVE_FILE"; \
			else \
				TOTAL_ERRORS="$$RELATIVE_FILE"; \
			fi; \
			FAILED_COUNT=$$(($$FAILED_COUNT + 1)); \
		else \
			echo "[PASS] $$(basename $${file%.in})"; \
			if [ $$PASSED_COUNT -gt 0 ]; then \
				TOTAL_PASSES="$${TOTAL_PASSES}\n$$RELATIVE_FILE"; \
			else \
				TOTAL_PASSES="$$RELATIVE_FILE"; \
			fi; \
			TOTAL_PASSES=$$TOTAL_PASSES"\n"$$(hyperfine $(HYPERFLAGS) --input $$file $(TARGET) | tail -n +2 | head -c -2); \
			PASSED_COUNT=$$(($$PASSED_COUNT + 1)); \
		fi; \
	done; \
	if [ -n "$$TOTAL_ERRORS" ]; then \
		echo "------------------- Failed Tests -------------------"; \
		echo "$$TOTAL_ERRORS"; \
	fi; \
	if [ -n "$$TOTAL_PASSES" ]; then \
		echo "------------------- Passed Tests -------------------"; \
		echo "$$TOTAL_PASSES"; \
	fi; \
	echo "<================ $$PASSED_COUNT/$$(($$PASSED_COUNT + $$FAILED_COUNT)) tests - $$(expr $$PASSED_COUNT \* 100 \/ \( $$PASSED_COUNT + $$FAILED_COUNT \))% =================>"; \

# Clean rule
clean:
	@rm -rf $(OUTDIR)
	@rm -rf $(REPORTDIR)
	@rm -f $(TESTDIR)/*.outme
	@rm -f $(TESTDIR)/*.diff

# Ensure the tests directory exists
$(TESTDIR):
	@if [ ! -d "$(TESTDIR)" ]; then \
		echo "The tests directory does not exist"; \
		exit 1; \
	fi

# Ensure the input and output files exist
$(TEST_INPUTS):
	@if [ -z "$(TEST_INPUTS)" ]; then \
		echo "No .in files found in $(TESTDIR)"; \
		exit 1; \
	fi

$(TEST_OUTPUTS):
	@if [ -z "$(TEST_OUTPUTS)" ]; then \
		echo "No .out files found in $(TESTDIR)"; \
		exit 1; \
	fi

# Ensure the output directory exists
$(OUTDIR):
	@mkdir -p $(OUTDIR)

# Ensure the report directory exists
$(REPORTDIR):
	@mkdir -p $(REPORTDIR)

# Compile and run checkers
$(TARGET): $(OUTDIR) $(SRC)
	@$(CC) $(CCLAGS) -o $(TARGET) $(SRC)

# Compile and run debug
$(DEBUGTARGET): $(OUTDIR)
	@$(CC) $(CCLAGS) -pg -o $(DEBUGTARGET) $(SRC)

# Run valgrind
valgrind: $(TARGET) $(REPORTDIR)
	@valgrind $(VALFLAGS) $(TARGET) < $(INPUT_FILE)

# Run valgrind Interactive
inter-valgrind: $(TARGET) $(REPORTDIR)
	@valgrind $(VALFLAGS) $(TARGET)

# Run gprof
gprof: $(DEBUGTARGET) $(REPORTDIR)
	$(DEBUGTARGET) < $(INPUT_FILE)
	@gprof $(DEBUGTARGET) $(GPROFFLAGS)
	@rm gmon.out

# Run gprof Interactive
inter-gprof: $(DEBUGTARGET) $(REPORTDIR)
	$(DEBUGTARGET)
	@gprof $(DEBUGTARGET) $(GPROFFLAGS)
	@rm gmon.out

# Run lizard
lizard: $(REPORTDIR)
	@lizard $(LIZFLAGS) $(SRC)

# Phony rules
.PHONY: all inter test clean valgrind inter-valgrind gprof inter-gprof lizard