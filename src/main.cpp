#include "Simulation.h"
#include <iostream>

using namespace std;

Simulation* backup = nullptr;

int main(int argc, char** argv){
    if(argc!=2){
        cout << "usage: simulation <config_path>" << endl;
        return 0;
    }
    string configurationFile = argv[1];
    std::cout << configurationFile << "\n\n\n";
    Simulation simulation(configurationFile);
    simulation.start();
    int x=5;
    int y = 7;
     if(backup!=nullptr){
     	delete backup;
     	backup = nullptr;
     }
    return 0;
}