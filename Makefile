CXX = dpcpp
CXXFLAGS = -O2 -std=c++17
EXE_NAME = vector-operation
SOURCES = src/main.cpp
DEVICE = EMU

all: build

build:
	$(CXX) $(CXXFLAGS) -o $(EXE_NAME) $(SOURCES) -DFPGA_${DEVICE}

run: 
	./$(EXE_NAME)

clean: 
	rm -f $(EXE_NAME)
