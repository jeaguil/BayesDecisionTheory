UNAME=$(shell uname)
SOURCEDIR=$(shell pwd)
CYGPATH=$(shell cygpath -w $(SOURCEDIR))

ifeq ($(UNAME), Linux)
	detected_OS=$(SOURCEDIR)
else
	detected_OS=$(CYGPATH)
endif
CXX=g++
CXXFLAGS=-std=c++14 -I $(detected_OS)
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