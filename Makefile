CFLAGS = -std=c++1y                                   # Compiler flags
DFLAGS =                                              # Debugueur flags
WFLAGS = -Wall -Wextra                                # Warning flags
LFLAGS = -lsfml-graphics -lsfml-window -lsfml-system  # Linker flags

EXEC = dungeon-battle
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

release:
	mkdir -p build
	g++ -c src/main.cpp -o build/main.o $(CFLAGS) $(DFLAGS) $(WFLAGS)
	g++ -o $(EXEC) build/main.o $(DFLAGS) $(LFLAGS)

debug: DFLAGS += -ggdb
debug: release

clean:
	rm -rf build/
