# Go game
Version 0.1

This is a Go game recreation using C++. Japanese ruleset is used.

## Instruction
Currently, the game needs C++ and its library in PATH environment variable to correctly link to the .dll files.

Run game.exe file to play.

## Build
Make use you have `makefile` to build.

Change the SFML path (`CFLAGS` and `LDFLAGS`) before build.

Run `make` (or `make all`) to build

## File tree

```bash
go-game
 ┣ 📂.vscode
 ┃ ┣ 📜settings.json
 ┃ ┗ 📜tasks.json
 ┣ 📂data
 ┃ ┗ 📜config.json
 ┣ 📂include
 ┃ ┣ 📜board.h
 ┃ ┣ 📜game_logic.h
 ┃ ┣ 📜game_scoring.h
 ┃ ┣ 📜test.h
 ┃ ┗ 📜zobrist_hash.h
 ┣ 📂lib
 ┃ ┗ 📜json.hpp
 ┣ 📂src
 ┃ ┣ 📜board.cpp
 ┃ ┣ 📜game_logic.cpp
 ┃ ┣ 📜game_scoring.cpp
 ┃ ┣ 📜main.cpp
 ┃ ┣ 📜test.cpp
 ┃ ┗ 📜zobrist_hash.cpp
 ┣ 📜.gitignore
 ┣ 📜Makefile
 ┗ 📜README.md
```

## Tech stack
Language: C++
- Compiler: g++ (Rev8, Built by MSYS2 project) 15.2.0.
- Build system: Makefile.

Graphic (not implemented): SFML 

AI (not implemented):
- Random - No judgement 
- Minimax - Shallow 
- Minimax + Alpha-Beta Pruning

## Makefile
```bash
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -I[SFML-PATH]\include -DSFML_STATIC -static -static-libgcc -static-libstdc++
LDFLAGS = -L[SFML-PATH]\lib
LIBS = -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-audio-s -lopengl32 -lfreetype -ljpeg -lwinmm -lgdi32 -lws2_32 -lopenal -lvorbis -lsfml-system -lflac -lvorbisenc -lvorbisfile -lvorbis -logg 

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET := game.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LDFLAGS) $(LIBS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET) $(TEST_TARGET)
```