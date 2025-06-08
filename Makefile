CXX := g++
CXXFLAGS := -Wall -Wextra -Werror -std=c++23 -Iheaders -Iheaders/utils -Itemplates 

SRC := src/main.cpp src/Parser.cpp src/Cube.cpp src/Algorithm.cpp src/Solver.cpp src/ParserUtils.cpp src/math.cpp src/printing.cpp

OBJDIR := build

OBJ := $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(SRC))

TARGET := rubik

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: src/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean
