CXXFLAGS = -std=c++14 -Wall -Wpedantic -Ofast -g
CXX = g++

OBJECTS = SplayTree.o MultiSplayTree.o WeightBalancedTree.o StdSetTree.o Timing.o PerfectlyBalancedTree.o HashTable.o SortedArray.o

default: run-tests explore

run-tests: RunTests.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

explore: Explore.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

RunTests.o: RunTests.cpp Timing.h StdSetTree.h SplayTree.h WeightBalancedTree.h Timer.h

Explore.o: Explore.cpp StdSetTree.h SplayTree.h WeightBalancedTree.h

PerfectlyBalancedTree.o: PerfectlyBalancedTree.cpp PerfectlyBalancedTree.h

SortedArray.o: SortedArray.cpp SortedArray.h

SplayTree.o: SplayTree.cpp SplayTree.h

StdSetTree.o: StdSetTree.cpp StdSetTree.h

HashTable.o: HashTable.cpp HashTable.h

MultiSplayTree.o: MultiSplayTree.cpp MultiSplayTree.h

Timing.o: Timing.cpp Timing.h

WeightBalancedTree.o: WeightBalancedTree.cpp WeightBalancedTree.h

clean:
	rm -f run-tests explore *.o *~
