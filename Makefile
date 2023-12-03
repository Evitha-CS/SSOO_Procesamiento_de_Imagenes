CXX = g++
CXXFLAGS = -std=c++11 -Wall -fopenmp $(shell pkg-config --cflags opencv4)
LDFLAGS = $(shell pkg-config --libs opencv4)

SRC = imagen_gris_secuencial.cpp
EXE = imagen_gris_secuencial

all: $(EXE)

$(EXE): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(EXE)
