CC=g++
CXX=g++
CXXFLAGS=-ggdb -Wall -O0 -std=c++17
ROOTFLAGS=`root-config --cflags --glibs`

# add new files here and its all good
objects=crossSection.o analysisManager.o eventObj.o \
	cut.o thetaCut.o vetoCut.o blscCut.o vertexCut.o \
	docaCut.o gemXYCut.o

# spec the analysis/ "master" bin to build
crossSection: $(objects)
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -o ../bin/crossSection $(objects)

# add others here for other analyses

# tell GNU how to make a .o from .cxx
# automatically generates dependancies
%.o: %.cxx 
	$(CXX) -MMD $(CXXFLAGS) $(ROOTFLAGS) -c $< 
	rm $*.d

.PHONY : clean

clean:
	-rm ../bin/crossSection *.o *.d
