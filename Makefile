CXX = dpcpp
CXXFLAGS =
EXE_NAME = vector-operation
SOURCES = src/main.cpp
DEVICE = EMU

all: build_emu

build_emu:
	$(CXX) $(CXXFLAGS) -o $(EXE_NAME) $(SOURCES) -DFPGA_EMU

build_hw:
	$(CXX) $(CXXFLAGS) -o $(EXE_NAME) $(SOURCES) -DFPGA_HW -fintelfpga -Xshardware

run: 
	./$(EXE_NAME)

clean: 
	rm -f $(EXE_NAME) *sh.o* *sh.e*
