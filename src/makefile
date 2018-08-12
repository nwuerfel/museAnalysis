CC = g++
CXX = g++
CXXFLAGS = -ggdb -Wall -O0 -std=c++17
ROOTFLAGS = `root-config --cflags --glibs`

crossSection: crossSection.o analysisManager.o
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -o ../bin/crossSection \
		crossSection.o analysisManager.o

crossSection.o: crossSection.cxx ../include/analysisManager.h
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -c crossSection.cxx

analysisManager.o: analysisManager.cxx ../include/analysisManager.h
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -c analysisManager.cxx

clean:
	-rm ../bin/crossSection crossSection.o analysisManager.o
