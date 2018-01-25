CXXFLAGS:=-std=c++11 -O2

EXEC = psutil

.all: ${EXEC}

${EXEC}: dp832.cpp main.cpp
	@c++ $(CXXFLAGS) $^ -o $@

.phony: clean
clean:
	rm ${EXEC}