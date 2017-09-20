# Compiler flags
CFLAGS = -std=c++1y

# Debuguer flags
DFLAGS =

# Warning flags
WFLAGS = -Wall -Wextra

# Linker flags
LFLAGS = -lsfml-graphics -lsfml-window -lsfml-system #-Wl,-rpath="$${ORIGIN}"

SRC_DIR = src
SRC = $(SRC_DIR)/main.cpp \
      $(SRC_DIR)/config.cpp
# List of files to compile

BUILD_DIR = build
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
# List of object files in build/

EXEC = dungeon-battle
# Executable name


SRC_DIR_TEST = tests
SRC_TEST = $(SRC_DIR_TEST)/test_configuration.cpp
OBJ_TEST = $(SRC_TEST:$(SRC_DIR_TEST)/%.cpp=$(BUILD_DIR_TEST)/%.o)
EXEC_TEST = $(SRC_TEST:$(SRC_DIR_TEST)/%.cpp=$(SRC_DIR_TEST)/%)

release: $(EXEC)

debug: DFLAGS += -ggdb
debug: $(EXEC)

$(EXEC): $(OBJ)
	g++ -o $@ $^ $(DFLAGS) $(LFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	g++ -o $@ -c $< $(CFLAGS) $(DFLAGS) $(WFLAGS)

tests: debug
tests: $(EXEC_TEST)

$(BUILD_DIR)/test_%.o: $(SRC_DIR_TEST)/test_%.cpp
	@mkdir -p $(BUILD_DIR)
	g++ -o $@ -c $< $(CFLAGS) $(DFLAGS) $(WFLAGS)

$(SRC_DIR_TEST)/%: $(BUILD_DIR)/%.o
	g++ -o $@ $< $(DFLAGS) $(LFLAGS)

doc:
	doxygen .doxygen.conf

all: tests doc release

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(BUILD_DIR_TEST)
	rm -rf dungeon-battle
	rm -rf doc/
	rm -rf *~
	rm -rf $(EXEC_TEST)

.PHONY: release debug tests doc all clean
