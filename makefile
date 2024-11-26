all: clean compile link run

compile:
	g++ -g -Wall -Weffc++ -c -Iinclude -o bin/settlement.o src/settlement.cpp
	g++ -g -Wall -Weffc++ -c -Iinclude -o bin/Facility.o src/Facility.cpp
	g++ -g -Wall -Weffc++ -c -Iinclude -o bin/testing.o src/testing.cpp
	g++ -g -Wall -Weffc++ -c -Iinclude -o bin/SelectionPolicy.o src/SelectionPolicy.cpp
	g++ -g -Wall -Weffc++ -c -Iinclude -o bin/Plan.o src/Plan.cpp
	g++ -g -Wall -Weffc++ -c -Iinclude -o bin/Simulation.o src/Simulation.cpp


link:
	g++ -o bin/hello bin/Facility.o bin/testing.o bin/Settlement.o bin/SelectionPolicy.o bin/Plan.o bin/Simulation.o -L/usr/lib

run:
	./bin/hello

clean:
	rm -f ./bin/main