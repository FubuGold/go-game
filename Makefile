CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -static -static-libgcc -static-libstdc++
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET := game.exe
TEST_TARGET := test.exe

TEST_OBJ := src/test.o
NON_TEST_OBJ := $(filter-out $(TEST_OBJ), $(OBJ))
all: $(TARGET)

$(TARGET): $(NON_TEST_OBJ)
	$(CXX) $(NON_TEST_OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET) $(TEST_TARGET)

test: $(TEST_TARGET)

MAIN_OBJ := src/main.o
NON_MAIN_OBJ := $(filter-out $(MAIN_OBJ), $(OBJ))
$(TEST_TARGET): $(NON_MAIN_OBJ)
	$(CXX) $(NON_MAIN_OBJ) -o $(TEST_TARGET)
	./$(TEST_TARGET)