CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude

.PHONY: all test run clean

all: lab2 tests_runner

lab2: src/main.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp -o lab2

tests_runner: tests/tests.cpp
	$(CXX) $(CXXFLAGS) tests/tests.cpp -o tests_runner

test: tests_runner
	./tests_runner

run: lab2
	./lab2

clean:
	rm -f lab2 tests_runner
