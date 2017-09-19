# Compiler flags
CFLAGS = -std=c++1y
# Debuguer flags
DFLAGS =
# Warning flags
WFLAGS = -Wall -Wextra
# Linker flags
LFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

EXEC = dungeon-battle

SRC_DIR = src
SRC = $(SRC_DIR)/main.cpp
# Liste des fichiers à compiler

BUILD_DIR = build
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
# Liste des objets dans build/

release: $(EXEC)

debug: DFLAGS += -ggdb
debug: $(EXEC)

$(EXEC): $(OBJ) $(SRC)
	g++ -o $(EXEC) $(OBJ) $(DFLAGS) $(LFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p build
	g++ -o $@ -c $< $(CFLAGS) $(DFLAGS) $(WFLAGS)

doc:
	doxygen .doxygen.conf

clean:
	rm -rf build/
	rm -rf dungeon-battle
