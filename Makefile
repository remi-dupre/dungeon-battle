# ==================================================================================================
# Variables

# C++ compiler name
CXX = g++

# Compiler flags
CFLAGS = -std=c++14

# Debuguer flags
DFLAGS =

# Warning flags
WFLAGS = -Wall -Wextra
WFLAGS_EXTRA = -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization \
               -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs \
               -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow \
               -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef
# Linker flags
LFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Source files
SRC_DIR = src
SRC = $(shell find $(SRC_DIR) -type f -name '*.cpp')

# Dependency files
DEP_DIR = deps
DEP_FILES = $(SRC:$(SRC_DIR)/%.cpp=$(DEP_DIR)/%.d)

# Object files
BUILD_DIR = build
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Dummy warning files
WARNINGS = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.w)

# Unit test paths
SRC_DIR_TEST = tests
SRC_TEST = $(shell find $(SRC_DIR_TEST) -type f -name 'test_*.hpp') # Test files
TEST_CPP = $(SRC_DIR_TEST)/tests.cpp
TEST_EXEC = $(SRC_DIR_TEST)/tests

# Executable name
EXEC = dungeon-battle

# Documentation directory
DOC_DIR = doc

# Dir where to output cppcheck reports
CHECK_DIR = check

# ==================================================================================================
# Configuration

.PHONY: all release debug tests-compile tests clean clean-all warnings \
	lint cppcheck-html doc package package-deb package-tar

# Include dependency files, without errors if they do not exist
NO_DEPS = clean clean-all warnings lint cppcheck-html doc package package-deb package-tar $(TEST_EXEC)
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NO_DEPS))))
        -include $(DEP_FILES)
endif

# ==================================================================================================
# Main targets

debug: DFLAGS += -ggdb
debug: $(EXEC)

release: CFLAGS += -O3 -DNDEBUG
release: $(EXEC)

all: release doc cppcheck-html tests

# ==================================================================================================
# Main rules

# Build executable from object files
$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(DFLAGS) $(LFLAGS)

# Build object file from source file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d
	@mkdir -p $(dir $@)
	$(CXX) -o $@ -c $< $(CFLAGS) $(DFLAGS) $(WFLAGS)

# Create dependency files
$(DEP_DIR)/%.d: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -MM -MT '$(<:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)' $< -MF $@

# ==================================================================================================
# Warnings target, output all g++ warnings without compiling
warnings: CFLAGS += -fsyntax-only
warnings: WFLAGS += $(WFLAGS_EXTRA)
warnings: $(WARNINGS)

# Output warnings
$(BUILD_DIR)/%.w: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $< $(CFLAGS) $(WFLAGS)

# ==================================================================================================
# Tests

# Compile and execute tests using cxxtest
tests: tests-compile
	$(TEST_EXEC) -v

# Compile test without executing them
tests-compile: $(TEST_CPP) $(filter-out $(BUILD_DIR)/main.o,$(OBJ))
	$(CXX) -o $(TEST_EXEC) $^ $(CFLAGS) $(WFLAGS) $(LFLAGS)

# Generate tests with cxxtest
$(TEST_CPP): $(SRC_TEST)
	cxxtestgen --error-printer -o $@ $^

# ==================================================================================================
# Static analysis of the code

# Target that cause Make to fail if an error is found
lint:
	cppcheck -q --enable=all --suppressions-list=.cppignore --inconclusive $(SRC_DIR) 1> /dev/null
	@# Return exit code 1 if there are errors
	cppcheck --error-exitcode=2 --suppressions-list=.cppignore --inconclusive $(SRC_DIR) 1> /dev/null 2> /dev/null

# Generate linter's report
cppcheck-html:
	cppcheck --enable=all --suppressions-list=.cppignore --inconclusive --xml --xml-version=2 $(SRC_DIR) 2> tmp_cppcheck.xml
	cppcheck-htmlreport --file=tmp_cppcheck.xml --report-dir=$(CHECK_DIR) --source-dir=.
	@rm tmp_cppcheck.xml

# ==================================================================================================
# Generate the documentation
doc:
	doxygen .doxygen.conf

# ==================================================================================================
# Create all packages
package:
	$(MAKE) clean
	$(MAKE) release
	$(MAKE) -C packages package-deb

	$(MAKE) clean
	$(MAKE) release
	$(MAKE) -C packages package-tar

# ==================================================================================================
# Clean intermediate files (not final results like executables, documentation, packages,...)
clean:
	rm -rf $(BUILD_DIR) $(BUILD_DIR)/generation
	rm -rf $(DEP_DIR)
	rm -rf packages/for-debian packages/for-tar
	rm -rf tests/test.cpp
	rm -rf *~

# Clean everything
clean-all: clean
	rm -rf dungeon-battle
	rm -rf packages/dungeon-battle.deb packages/dungeon-battle.tar.gz
	rm -rf $(DOC_DIR)
	rm -rf $(CHECK_DIR)
	rm -rf $(TEST_EXEC)
