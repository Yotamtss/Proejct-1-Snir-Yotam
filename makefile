# All Targets
all: simulation

# Tool invocations
# Executable “simulation” depends on the object files of all the source files.
simulation: bin/Simulation.o bin/Plan.o bin/Settlements.o bin/SelectionPolicy.o bin/Facility.o
	@echo 'Building target: simulation'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/simulation bin/Simulation.o bin/Plan.o bin/Settlements.o bin/SelectionPolicy.o bin/Facility.o
	@echo 'Finished building target: simulation'
	@echo ''

# Depends on the source and header files for Simulation
bin/Simulation.o: src/Simulation.cpp
	@echo 'Building object file: Simulation.o'
	g++ -g -Wall -Wextra -std=c++11 -c -Iinclude -o bin/Simulation.o src/Simulation.cpp

# Depends on the source and header files for Plan
bin/Plan.o: src/Plan.cpp
	@echo 'Building object file: Plan.o'
	g++ -g -Wall -Wextra -std=c++11 -c -Iinclude -o bin/Plan.o src/Plan.cpp

# Depends on the source and header files for Settlements
bin/Settlements.o: src/Settlements.cpp
	@echo 'Building object file: Settlements.o'
	g++ -g -Wall -Wextra -std=c++11 -c -Iinclude -o bin/Settlements.o src/Settlements.cpp

# Depends on the source and header files for SelectionPolicy
bin/SelectionPolicy.o: src/SelectionPolicy.cpp
	@echo 'Building object file: SelectionPolicy.o'
	g++ -g -Wall -Wextra -std=c++11 -c -Iinclude -o bin/SelectionPolicy.o src/SelectionPolicy.cpp

# Depends on the source and header files for Facility
bin/Facility.o: src/Facility.cpp
	@echo 'Building object file: Facility.o'
	g++ -g -Wall -Wextra -std=c++11 -c -Iinclude -o bin/Facility.o src/Facility.cpp

# Clean the build directory
clean:
	rm -f bin/*
