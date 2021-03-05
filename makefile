CXX=g++
CXXFLAGS=-std=c++11
project=classification
OBJDIR=obj

srcfiles=$(shell find -name "*.cpp")
objects=$(subst .cpp,.o,$(srcfiles))

all: $(project)

$(project): $(objects)
	$(CXX) $(CXXFLAGS) -o $(project) $(objects)

clean:
	rm -f $(objects)

dist-clean: clean
	rm -f $(project)