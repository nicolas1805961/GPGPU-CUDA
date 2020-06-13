CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17 -g -fsanitize=address -Icpu/include
LDFLAGS =  -fsanitize=address

GXX = nvcc
GXXFLAGS = -Igpu/include

GSRC = gpu/src/main.cu \
	   gpu/src/Graph_gpu.cu \
	   gpu/src/Image.cpp
GEXEC = GPU


SRC = cpu/src/main.cpp \
	  cpu/src/Graph.cpp \
	  cpu/src/Image.cpp \
	  cpu/src/graphcut_cpu.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = CPU

all: $(EXEC) $(GEXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

$(GEXEC):
	$(GXX) $(GXXFLAGS) -o $@ $(GSRC)

clean:
	rm -rf $(OBJ) $(EXEC) $(GEXEC)
