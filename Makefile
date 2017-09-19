CFLAGS = -std=c++1z                                   # Compiler flags
DFLAGS =                                              # Debugueur flags
WFLAGS = -Wall -Wextra                                # Warning flags
LFLAGS = -lsfml-graphics -lsfml-window -lsfml-system  # Linker flags

SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

release:
	mkdir -p build
	g++ -ggdb -c src/main.cpp -o build/main.o $(CFLAGS) $(DFLAGS) $(WFLAGS)
	g++ -ggdb build/main.o $(DFLAGS) $(LFLAGS)

debug: DFLAGS += -ggdb
debug: release

clean:
	rm -rf build/
