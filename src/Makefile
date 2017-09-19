WFLAGS = -Wall -Wextra
LFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
LDFLAGS = -lsfml-graphics-d -lsfml-window-d -lsfml-system-d

release:
	g++ -ggdb -c main.cpp $(WFLAGS)
	g++ -ggdb main.o $(LFLAGS)

debug:
	g++ -ggdb -c main.cpp $(WFLAGS)
	g++ -ggdb main.o $(LDFLAGS)
