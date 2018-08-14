CC = g++
CXX = g++
CXXFLAGS = -ggdb -Wall -O0 -std=c++17
ROOTFLAGS = `root-config --cflags --glibs`

crossSection: crossSection.o analysisManager.o eventObj.o cut.o \
	thetaCut.o
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -o ../bin/crossSection \
		crossSection.o analysisManager.o eventObj.o cut.o \
			thetaCut.o

crossSection.o: crossSection.cxx ../include/analysisManager.h
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -c crossSection.cxx

analysisManager.o: analysisManager.cxx ../include/analysisManager.h 
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -c analysisManager.cxx

eventObj.o: eventObj.cxx ../include/eventObj.h
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -c eventObj.cxx

cut.o: cut.cxx ../include/cut.h
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -c cut.cxx

thetaCut.o: thetaCut.cxx ../include/thetaCut.h
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -c thetaCut.cxx

clean:
	-rm ../bin/crossSection *.o
