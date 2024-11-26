#include "Simulation.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

/* 
// Rule of 5 Implementation
Simulation::Simulation(const Simulation& other) {
    *this = other; // Delegate to copy assignment
}

Simulation& Simulation::operator=(const Simulation& other) {
    if (this != &other) {
        isRunning = other.isRunning;
        planCounter = other.planCounter;

        // Deep copy actions
        for (auto action : actionsLog) delete action;
        actionsLog.clear();
        for (auto action : other.actionsLog) {
            //actionsLog.push_back(action->clone());
        }

        plans = other.plans; // Assume Plan implements Rule of 5
        settlements = other.settlements;
        facilitiesOptions = other.facilitiesOptions;
    }
    return *this;
}

Simulation::Simulation(Simulation&& other) noexcept
    : isRunning(other.isRunning),
      planCounter(other.planCounter),
      actionsLog(std::move(other.actionsLog)),
      plans(std::move(other.plans)),
      settlements(std::move(other.settlements)),
      facilitiesOptions(std::move(other.facilitiesOptions)) {
    other.isRunning = false;
    other.planCounter = 0;
}

Simulation& Simulation::operator=(Simulation&& other) noexcept {
    if (this != &other) {
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        actionsLog = std::move(other.actionsLog);
        plans = std::move(other.plans);
        settlements = std::move(other.settlements);
        facilitiesOptions = std::move(other.facilitiesOptions);

        other.isRunning = false;
        other.planCounter = 0;
    }
    return *this;
}
*/
Simulation::~Simulation() {
    for (auto action : actionsLog) delete action;
    for (auto settlement : settlements) delete settlement;

}

// Constructor: Parse Config File
Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0) {
    std::ifstream configFile(configFilePath);
    if (!configFile) {
        throw std::runtime_error("Failed to open config file: " + configFilePath);
    }

    string line;
    while (std::getline(configFile, line)) {
        std::istringstream iss(line);
        string command;
        iss >> command;

        if (command == "settlement") {
            string name;
            int type;
            iss >> name >> type;
            Settlement toAdd = Settlement(name, static_cast<SettlementType>(type));
            addSettlement(&toAdd);
        } else if (command == "facility") {
            string name;
            int category, price, lifeq, eco, env;
            iss >> name >> category >> price >> lifeq >> eco >> env;
            addFacility(FacilityType(name, static_cast<FacilityCategory>(category), price, lifeq, eco, env));
        } else if (command == "plan") {
            string settlementName, policyAbbr;
            iss >> settlementName >> policyAbbr;
            SelectionPolicy *policy = nullptr;

            if (policyAbbr == "nve") policy = new NaiveSelection();
            else if (policyAbbr == "bal") policy = new BalancedSelection(0, 0, 0);
            else if (policyAbbr == "eco") policy = new EconomySelection();
            else if (policyAbbr == "env") policy = new SustainabilitySelection();

            if (policy && isSettlementExists(settlementName)) {
                addPlan(getSettlement(settlementName), policy);
            } else {
                throw std::runtime_error("Invalid plan configuration.");
            }
        }
    }
    start();
}
void Simulation::start() {
    isRunning = true;
    std::cout << "The Simulation has started" << std::endl;
}

// Add a plan to the simulation
void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy) {
    if (!isSettlementExists(settlement.getName())) {
        throw std::runtime_error("Settlement does not exist: " + settlement.getName());
    }
    plans.emplace_back(planCounter++, &settlement, selectionPolicy, facilitiesOptions);
}

// Add an action to the simulation
void Simulation::addAction(BaseAction *action) {
    if (!action) {
        throw std::runtime_error("Invalid action.");
    }
    actionsLog.push_back(action);
}

// Add a settlement to the simulation
bool Simulation::addSettlement(Settlement *settlement) {
    if (isSettlementExists(settlement->getName())) {
        return false; // Settlement already exists
    }
    settlements.push_back(settlement);
    return true;
}

Settlement &Simulation::getSettlement(const string &settlementName)
{
    for(Settlement *stl : settlements)
    {
        if(stl->getName() == settlementName)
            return *stl;
    }
} 
Plan &Simulation::getPlan(const int planID)
{
    for(Plan plan : plans)
    {
        if(plan.getID() == planID)
            return plan;
    }
} 

// Add a facility to the simulation
bool Simulation::addFacility(FacilityType facility) {
    for (const auto &existingFacility : facilitiesOptions) {
        if (existingFacility.getName() == facility.getName()) {
            return false; // Facility already exists
        }
    }
    facilitiesOptions.push_back(facility);
    return true;
}

// Check if a settlement exists
bool Simulation::isSettlementExists(const string &settlementName) {
    return std::any_of(settlements.begin(), settlements.end(),
                       [&settlementName](const Settlement &s) {
                           return s.getName() == settlementName;
                       });
}

void Simulation::step(){
    for(auto plan : plans)
    {
        plan.step();
    }
}

void Simulation::open()
{
    isRunning = true;
}

void Simulation::close()
{
    for(auto plan : plans)
    {
        plan.printStatus();
        plan.~Plan();
    }
    isRunning = false;
}



