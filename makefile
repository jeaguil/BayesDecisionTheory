UNAME=$(shell uname)
SOURCEDIR=$(shell pwd)
CYGPATH=$(shell cygpath -w $(SOURCEDIR))

ifeq ($(UNAME), Linux)
	CXXFLAGS=-std=c++14 -I $(SOURCEDIR)
else
	CXXFLAGS=-std=c++14 -I $(CYGPATH)
endif
CXX=g++
project=classification

srcfiles=$(shell find -name "*.cpp")
objects=$(subst .cpp,.o,$(srcfiles))

all: $(project)

$(project): $(objects)
	$(CXX) $(CXXFLAGS) -o $(project) $(objects)

clean:
	rm -f $(objects)

dist-clean: clean
	rm -f $(project)