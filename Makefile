CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -IC:\SFML-3.0.2\include -DSFML_STATIC -static -static-libgcc -static-libstdc++
LDFLAGS = -LC:\SFML-3.0.2\lib
LIBS = -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -ljpeg -lwinmm -lgdi32 -lws2_32

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