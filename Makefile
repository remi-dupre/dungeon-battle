# C++ compiler name
CXX = g++

# Compiler flags
CFLAGS = -std=c++1z

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
      $(SRC_DIR)/args.cpp
# List of files to compile

BUILD_DIR = build
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
# List of object files in build/

EXEC = dungeon-battle
# Executable name

# Documentation directory
DOC_DIR = doc

SRC_DIR_TEST = tests
SRC_TEST = $(SRC_DIR_TEST)/test_configuration.cpp \
           $(SRC_DIR_TEST)/test_parse_arguments.cpp
OBJ_TEST = $(SRC_TEST:$(SRC_DIR_TEST)/%.cpp=$(BUILD_DIR_TEST)/%.o)
EXEC_TEST = $(SRC_TEST:$(SRC_DIR_TEST)/%.cpp=$(SRC_DIR_TEST)/%)


.PHONY: all release debug test doc clean

all: release doc test

release: CFLAGS += -O3 -DNDEBUG
release: $(EXEC)

debug: DFLAGS += -ggdb
debug: $(EXEC)

test: debug
test: $(EXEC_TEST)

# Generate the documentation
doc:
	doxygen .doxygen.conf

# Build object file from source file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.hpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) -o $@ -c $< $(CFLAGS) $(DFLAGS) $(WFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) -o $@ -c $< $(CFLAGS) $(DFLAGS) $(WFLAGS)

# Build executable from object files
$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(DFLAGS) $(LFLAGS)

# Build test object file from source file
$(BUILD_DIR)/test_%.o: $(SRC_DIR_TEST)/test_%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) -o $@ -c $< $(CFLAGS) $(DFLAGS) $(WFLAGS)

# Build and execute test
$(SRC_DIR_TEST)/%: $(BUILD_DIR)/%.o
	$(CXX) -o $@ $< $(DFLAGS) $(LFLAGS)
	@echo "Running the test: " $(@:$(SRC_DIR_TEST)/%=%)
	@cd $(SRC_DIR_TEST) && ./$(@:$(SRC_DIR_TEST)/%=%)

# Clean the workspace
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(DOC_DIR)
	rm -rf $(EXEC_TEST)
	rm -rf dungeon-battle
	rm -rf *~
