#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "Simulation.h"
#include "Auxiliary.h"
#include "Settlement.h"
#include "Facility.h"
#include "SelectionPolicy.h"
#include "Action.h"
#include "Plan.h"
#include <sstream>
using namespace std;

extern Simulation *backup;

// Constructor: Parse Config File
Simulation::Simulation(const string &configFilePath) : isRunning(false), // Initialize to false
      planCounter(0),   // Initialize to 0
      actionsLog(),     // Default initialize as an empty vector
      settlements(),    // Default initialize as an empty vector
      facilitiesOptions(), // Default initialize as an empty vector
      plans()            {
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        throw std::runtime_error("Failed to open config file: " + configFilePath);
    }

    string line;
    while (std::getline(configFile, line)) {
        // Trim leading/trailing whitespaces
        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);

        // Skip empty lines or comments
        if (line.empty() || line[0] == '#')
        {
            continue;
        }
        
        actionHandler(line); // Handle the full line of input        
    }
    
    for (BaseAction *action : actionsLog)
    {
        if(action)
           delete action;
        action=nullptr;
    }

    actionsLog.clear();
    planCounter = plans.size();
    configFile.close();
}


void Simulation::start() {
    isRunning = true;
    std::string action = "";

    std::cout << "The Simulation has started" << std::endl;

        while (isRunning)
    {
        std::cout << "Type an action (or 'close' to stop): ";
        std::getline(std::cin, action); // Use getline to capture the entire input line

        actionHandler(action); // Handle the full line of input
    }

    delete backup;
    backup = nullptr;

    isRunning = false; // Mark simulation as stopped
}

// Add a plan to the simulation
void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy) {
    plans.emplace_back(planCounter, settlement, selectionPolicy, facilitiesOptions);
    planCounter++;
}

// Add an action to the simulation
void Simulation::addAction(BaseAction *action) {
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
        throw std::runtime_error("Settlement not found");
} 

Plan &Simulation::getPlan(const int planID)
{
if (planID < 0 || static_cast<size_t>(planID) >= plans.size()) 
    {
        std::cout << "Invalid plan ID" << std::endl;
    }
    // Return the plan by reference
    return plans[planID];
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
    for (Settlement *settl : settlements)
    {
        if (settl->getName() == settlementName)
        {
            return true;
        } 
    }
    return false;
}

void Simulation::step(){
    for(Plan &plan : plans)
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
    for(Plan &plan: plans)
    {
        plan.printStatus();
    }
    isRunning = false;
}

void Simulation::actionHandler(const std::string &action)
{
    std::vector<std::string> words = Auxiliary::parseArguments(action);

    if (words[0] == "log")
    {
        PrintActionsLog printLog = PrintActionsLog();
        printLog.act(*this);
        BaseAction *clonedRestore = printLog.clone();
        actionsLog.push_back(clonedRestore);
    }

    if (words[0] == "settlement")
    {
        SettlementType type;
        if(words[2] == "0") type = SettlementType::VILLAGE;
        else if(words[2] == "1") type = SettlementType::CITY;
        else if(words[2] == "2") type = SettlementType::METROPOLIS;
        AddSettlement settlemntToBeAdded = AddSettlement(words[1], type);
        settlemntToBeAdded.act(*this);
        BaseAction *clonedRestore = settlemntToBeAdded.clone();
        actionsLog.push_back(clonedRestore);
    }
    else if (words[0] == "facility")
    {
        FacilityCategory cat;
        if(words[2] == "0") cat = FacilityCategory::LIFE_QUALITY;
        else if(words[2] == "1") cat = FacilityCategory::ECONOMY;
        else if(words[2] == "2") cat = FacilityCategory::ENVIRONMENT;
        AddFacility faccilityToBeAdded = AddFacility(words[1], cat, std::stoi(words[3]), std::stoi(words[4]), std::stoi(words[5]), std::stoi(words[6]));
        faccilityToBeAdded.act(*this);
        BaseAction *clonedRestore = faccilityToBeAdded.clone();
        actionsLog.push_back(clonedRestore);

    }
        else if (words[0] == "plan")
    {
        AddPlan planToBeAdded(words[1], words[2]);
        planToBeAdded.act(*this);
        BaseAction *clonedRestore = planToBeAdded.clone();
        actionsLog.push_back(clonedRestore);
    }

    else if (words[0] == "planStatus")
    {
        PrintPlanStatus planStatusToBeAdded = PrintPlanStatus(std::stoi(words[1]));
        planStatusToBeAdded.act(*this);
        BaseAction *clonedRestore = planStatusToBeAdded.clone();
        actionsLog.push_back(clonedRestore);
    }
    else if (words[0] == "step")
    {
        SimulateStep simulateStepToBeAdded = SimulateStep(std::stoi(words[1]));
        simulateStepToBeAdded.act(*this);
        BaseAction *clonedRestore = simulateStepToBeAdded.clone();
        actionsLog.push_back(clonedRestore);
    }
    else if (words[0] == "changePolicy")
    {
        ChangePlanPolicy changePlanPolicyToBeAdded = ChangePlanPolicy(std::stoi(words[1]), words[2]);
        changePlanPolicyToBeAdded.act(*this);
        BaseAction *clonedRestore = changePlanPolicyToBeAdded.clone();
        actionsLog.push_back(clonedRestore);
    }
    else if(words[0] == "close")
    {
        Close close = Close();
        close.act(*this);
    }
    else if (words[0] == "restore")
    {
        RestoreSimulation restore = RestoreSimulation();
        restore.act(*this);
        BaseAction *clonedRestore = restore.clone();
        actionsLog.push_back(clonedRestore);
    }

    else if (words[0] == "backup")
    {
        BackupSimulation backupSim = BackupSimulation();
        backupSim.act(*this);
        BaseAction *clonedRestore = backupSim.clone();
        actionsLog.push_back(clonedRestore);
    }

}

void Simulation::printLog() const
{
    for (BaseAction *action : actionsLog)
    {
        std::cout << action->toString() << std::endl;
    }
}


int & Simulation::getplanCounter()
{
    return planCounter;
}

const vector<BaseAction*> & Simulation::getActionsLog()
{
    return actionsLog;
} 

// ***********************
//RULE OF 5 IMPLEMENTATION
// ***********************

Simulation::Simulation(const Simulation &other)
    : isRunning(other.isRunning),
      planCounter(other.planCounter),
      actionsLog(),
      settlements(),
      facilitiesOptions(other.facilitiesOptions),
      plans()
{
    // Deep copy actionsLog
    for (auto *action : other.actionsLog)
    {
        actionsLog.push_back(action->clone());
    }

    // Deep copy settlements
    for (auto *settlement : other.settlements)
    {
        settlements.push_back(new Settlement(*settlement));
    }

    // Deep copy plans
    for (const auto &plan : other.plans)
    {
        const std::string &settlementName = plan.getSettlement();
        Settlement *newSettlement = nullptr;

        // Find corresponding settlement
        for (Settlement *copiedSettlement : settlements)
        {
            if (copiedSettlement->getName() == settlementName)
            {
                newSettlement = copiedSettlement;
                break;
            }
        }

        // Deep copy facilities
        std::vector<Facility *> copiedFacilities;
        for (Facility *facility : plan.getFacilities())
        {
            copiedFacilities.push_back(facility->clone());
        }

        // Deep copy underConstruction
        std::vector<Facility *> copiedUnderConstruction;
        for (Facility *facility : plan.getConstruction())
        {
            copiedUnderConstruction.push_back(facility->clone());
        }

        // Reconstruct the Plan
        plans.emplace_back(
            plan.getID(),
            *newSettlement, // Deep-copied settlement
            plan.getPolicy() ? plan.getPolicy()->clone() : nullptr,
            facilitiesOptions,
            plan.getlifeQualityScore(),
            plan.getEconomyScore(),
            plan.getEnvironmentScore(),
            std::move(copiedFacilities),       // Transfer ownership
            std::move(copiedUnderConstruction) // Transfer ownership
        );
    }
}

Simulation &Simulation::operator=(const Simulation &other)
{
    if (this == &other)
    {
        return *this; // Prevent self-assignment
    }

    // Cleanup current resources
    for (BaseAction *action : actionsLog)
    {
        delete action;
    }
    actionsLog.clear();

    for (Settlement *settlement : settlements)
    {
        delete settlement;
    }
    settlements.clear();

    plans.clear();
    facilitiesOptions.clear();

    // Copy basic members
    isRunning = other.isRunning;
    planCounter = other.planCounter;
    for (const FacilityType &facility : other.facilitiesOptions)
    {
        facilitiesOptions.push_back(facility); // Uses copy constructor
    }
    // Deep copy actionsLog
    for (BaseAction *action : other.actionsLog)
    {
        actionsLog.push_back(action->clone());
    }

    // Deep copy settlements
    for (Settlement *settlement : other.settlements)
    {
        settlements.push_back(new Settlement(*settlement));
    }

    // Deep copy plans
    for (const auto &plan : other.plans)
    {
        Settlement *newSettlement = nullptr;
        for (Settlement *copiedSettlement : settlements)
        {
            if (copiedSettlement->getName() == plan.getSettlement())
            {
                newSettlement = copiedSettlement;
                break;
            }
        }

        std::vector<Facility *> copiedFacilities;
        for (Facility *facility : plan.getFacilities())
        {
            copiedFacilities.push_back(facility->clone());
        }

        std::vector<Facility *> copiedUnderConstruction;
        for (Facility *facility : plan.getConstruction())
        {
            copiedUnderConstruction.push_back(facility->clone());
        }

        plans.emplace_back(
            plan.getID(),
            *newSettlement,
            plan.getPolicy() ? plan.getPolicy()->clone() : nullptr,
            facilitiesOptions,
            plan.getlifeQualityScore(),
            plan.getEconomyScore(),
            plan.getEnvironmentScore(),
            std::move(copiedFacilities),
            std::move(copiedUnderConstruction));
    }

    return *this;
}

Simulation::Simulation(Simulation&& other) noexcept
    : isRunning(other.isRunning),
      planCounter(other.planCounter),
      actionsLog(std::move(other.actionsLog)),
      settlements(std::move(other.settlements)),
      facilitiesOptions(std::move(other.facilitiesOptions)),
      plans(std::move(other.plans)){
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

Simulation::~Simulation() {

    for (BaseAction *action : actionsLog)
    {
        if(action)
           delete action;
        action=nullptr;
    }
    for (Settlement *settlement : settlements)
    {
        if(settlement)
            delete settlement;
        settlement = nullptr;
    }
    actionsLog.clear();
    settlements.clear();
    plans.clear();
    facilitiesOptions.clear();

}