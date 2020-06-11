CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17 -g -fsanitize=address -Icpu/include
LDFLAGS =  -fsanitize=address

SRC = cpu/src/main.cpp \
	  cpu/src/Graph.cpp \
	  cpu/src/Image.cpp \
	  cpu/src/graphcut_cpu.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = CPU

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

clean:
	rm -rf $(OBJ) $(EXEC)
