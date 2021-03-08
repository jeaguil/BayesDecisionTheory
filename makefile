SOURCEDIR = $(shell pwd)
CXX=g++
CXXFLAGS=-std=c++14 -I $(SOURCEDIR)
project=classification

srcfiles=$(shell find -name "*.cpp")
objects=$(subst .cpp,.o,$(srcfiles))

all: $(project)

$(project): $(objects)
	$(CXX) $(CXXFLAGS) -o $(project) $(objects)

clean:
	rm -f $(objects)

subdir_out:
	$(subdir)

dist-clean: clean
	rm -f $(project)