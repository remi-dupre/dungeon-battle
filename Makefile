# C++ compiler name
CXX = g++

# Compiler flags
CFLAGS = -std=c++1y

# Debuguer flags
DFLAGS =

# Warning flags
WFLAGS = -Wall -Wextra

# Linker flags
LFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -Wl,-rpath,.

SRC_DIR = src
SRC = $(SRC_DIR)/main.cpp \
      $(SRC_DIR)/map.cpp \
      $(SRC_DIR)/render.cpp \
      $(SRC_DIR)/config.cpp \
      $(SRC_DIR)/game.cpp \
      $(SRC_DIR)/entity.cpp \
      $(SRC_DIR)/args.cpp \
      $(SRC_DIR)/control.cpp \
      $(SRC_DIR)/ai.cpp \
      $(SRC_DIR)/utility.cpp \
      $(SRC_DIR)/generation/pattern.cpp \
      $(SRC_DIR)/generation/room.cpp \
      $(SRC_DIR)/generation/level.cpp \
      $(SRC_DIR)/generation/gen_pattern.cpp \
      $(SRC_DIR)/generation/space.cpp \
      $(SRC_DIR)/generation/generator.cpp \
      $(SRC_DIR)/menu.cpp

# List of files to compile

BUILD_DIR = build
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
# List of object files in build/

EXEC = dungeon-battle
# Executable name

# Documentation directory
DOC_DIR = doc

# Dir where to output cppcheck reports
CHECK_DIR = check

# Unit test paths
SRC_DIR_TEST = tests
SRC_TEST = $(SRC_DIR_TEST)/test_map.hpp \
           $(SRC_DIR_TEST)/test_config.hpp \
           $(SRC_DIR_TEST)/test_generation.hpp \
           $(SRC_DIR_TEST)/test_kdtree.hpp \
           $(SRC_DIR_TEST)/test_pattern.hpp

# Files the tests depend of
DPND_TEST = $(filter-out $(SRC_DIR)/main.cpp,$(SRC))

OBJ_TEST = $(SRC_DIR_TEST)/test.cpp

TEST_EXEC = $(SRC_DIR_TEST)/test


.PHONY: all release debug test doc cppcheck-html clean

all: release

release: CFLAGS += -O3 -DNDEBUG
release: $(EXEC)

debug: DFLAGS += -ggdb
debug: $(EXEC)

rebuild: clean
rebuild: release

lint:
	cppcheck --enable=all --suppressions-list=.cppignore --inconclusive $(SRC_DIR) 1> /dev/null

# Exectutes tests using cxxtest
test: $(OBJ_TEST) $(SRC)

	$(CXX) -o $(TEST_EXEC) $(OBJ_TEST) $(DPND_TEST) $(CFLAGS) $(DFLAGS) $(WFLAGS) $(LFLAGS)
	$(TEST_EXEC) -v

# Build the test cpp
$(OBJ_TEST): $(SRC_TEST)
	cxxtestgen --error-printer -o tests/test.cpp $(SRC_TEST)

# Generate the documentation
doc:
	doxygen .doxygen.conf

# Generate linter's report
cppcheck-html:
	cppcheck --enable=all --suppressions-list=.cppignore --inconclusive --xml $(SRC_DIR) 2> tmp_cppcheck.xml
	cppcheck-htmlreport --file=tmp_cppcheck.xml --report-dir=$(CHECK_DIR) --source-dir=.
	@rm tmp_cppcheck.xml

# Build object file from source file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.hpp
	@mkdir -p $(BUILD_DIR) $(BUILD_DIR)/generation
	$(CXX) -o $@ -c $< $(CFLAGS) $(DFLAGS) $(WFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR) $(BUILD_DIR)/generation
	$(CXX) -o $@ -c $< $(CFLAGS) $(DFLAGS) $(WFLAGS)

# Build executable from object files
$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(DFLAGS) $(LFLAGS)

# Build and execute test
$(SRC_DIR_TEST)/%: $(BUILD_DIR)/%.o
	$(CXX) -o $@ $< $(DFLAGS) $(LFLAGS)
	@echo "Running the test: " $(@:$(SRC_DIR_TEST)/%=%)
	@cd $(SRC_DIR_TEST) && ./$(@:$(SRC_DIR_TEST)/%=%)

# Clean the workspace
clean:
	rm -rf $(BUILD_DIR) $(BUILD_DIR)/generation
	rm -rf $(DOC_DIR)
	rm -rf $(EXEC_TEST)
	rm -rf $(CHECK_DIR)
	rm -rf tests/test.cpp tests/test
	rm -rf dungeon-battle
	rm -rf *~
