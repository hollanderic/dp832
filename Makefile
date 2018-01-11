


CXXFLAGS:=-std=c++11
SOURCES:=dp832.cpp \
		 main.cpp

.all: psutil

.phony: psutil

psutil: dp832.cpp main.cpp
	@c++ $(CXXFLAGS) $^ -o $@
