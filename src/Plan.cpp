#include "Plan.h"
#include <iostream>
#include "SelectionPolicy.h"

// Constructor
Plan::Plan(const int planId, const Settlement *settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId)
    , settlement(settlement)
    , selectionPolicy(selectionPolicy)
    , status(PlanStatus::AVALIABLE)
    , facilities()
    , underConstruction()
    , facilityOptions(facilityOptions)
    , life_quality_score(0)
    , economy_score(0)
    , environment_score(0)
{
}

const int Plan::getlifeQualityScore() const {
    return life_quality_score;
}

const int Plan::getEconomyScore() const {
    return economy_score;
}

const int Plan::getEnvironmentScore() const {
    return environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *newSelectionPolicy) {
    if (selectionPolicy != nullptr) {
        delete selectionPolicy;
    }
    selectionPolicy = newSelectionPolicy;
}

void Plan::step() {
    // Stage 1: Check if plan is BUSY
    if (status != PlanStatus::BUSY) {
        // Stage 2: Select and add new facilities if possible
        while (underConstruction.size() < settlement->getConstructionLimit() && status != PlanStatus::BUSY) {
            // Select facility based on current policy
            Facility* newFacility = new Facility(selectionPolicy->selectFacility(facilityOptions), settlement->getName());
            underConstruction.push_back(newFacility);
        }
    }

    // Stage 3: Process facilities under construction
    vector<Facility*> completedFacilities;
    for (int i = underConstruction.size() - 1; i >= 0; i--)
    {
        Facility* facility = underConstruction[i];
        FacilityStatus facilityStatus = facility->step();
        if (facilityStatus == FacilityStatus::OPERATIONAL) {
            // Update scores
            life_quality_score += facility->getLifeQualityScore();
            economy_score += facility->getEconomyScore();
            environment_score += facility->getEnvironmentScore();
            
            // Move to completed facilities
            facilities.push_back(facility);
            underConstruction.erase(underConstruction.begin() + i);

    }

    }

    // Stage 4: Update plan status
    status = (underConstruction.size() >= settlement->getConstructionLimit()) 
        ? PlanStatus::BUSY 
        : PlanStatus::AVALIABLE;
}

void Plan::printStatus() {
    std::cout << toString();
}

const vector<Facility*> &Plan::getFacilities() const {
    return facilities;
}

const int Plan::getID() const {
    return plan_id;
}

void Plan::addFacility(Facility* facility) {
    facilities.push_back(facility);
    // Update scores
    life_quality_score += facility->getLifeQualityScore();
    economy_score += facility->getEconomyScore();
    environment_score += facility->getEnvironmentScore();
}


const string Plan::toString() const {
    string result = "planID: " + std::to_string(plan_id) + " settlementName: " + settlement->getName() + "\n";
    result += "planStatus: " + string(status == PlanStatus::BUSY ? "BUSY" : "AVALIABLE") + "\n";
    
    // Selection policy string representation
    string policyStr;
    if (dynamic_cast<NaiveSelection*>(selectionPolicy)) policyStr = "nve";
    else if (dynamic_cast<BalancedSelection*>(selectionPolicy)) policyStr = "bal";
    else if (dynamic_cast<EconomySelection*>(selectionPolicy)) policyStr = "eco";
    else if (dynamic_cast<SustainabilitySelection*>(selectionPolicy)) policyStr = "env";
    
    result += "selectionPolicy: " + policyStr + "\n";
    result += "LifeQualityScore: " + std::to_string(life_quality_score) + "\n";
    result += "EconomyScore: " + std::to_string(economy_score) + "\n";
    result += "EnvironmentScore: " + std::to_string(environment_score) + "\n";

/*    // Sort facilities by ID before printing
    vector<Facility*> allFacilities = facilities;
    allFacilities.insert(allFacilities.end(), underConstruction.begin(), underConstruction.end());
    std::sort(allFacilities.begin(), allFacilities.end(), 
        [](const Facility* a, const Facility* b) {
            return a->getName() < b->getName();
        });

    // Print all facilities
    for (const Facility* facility : allFacilities) {
        result += facility->toString() + "\n";
    }
*/
    return result;

}

Plan::~Plan() {
    // Clean up dynamically allocated selection policy
    if (selectionPolicy) {
        delete selectionPolicy;
    }

    // Clean up dynamically allocated facilities
    for (Facility* facility : underConstruction) {
        delete facility;
    }
    underConstruction.clear();

    for (Facility* facility : facilities) {
        delete facility;
    }
    facilities.clear();
}

Plan::Plan(const Plan& other) 
    : plan_id(other.plan_id)
    , settlement(other.settlement) // Settlement assumed to be a raw pointer, copied as-is
    , selectionPolicy(other.selectionPolicy ? other.selectionPolicy->clone() : nullptr) // Clone policy
    , status(other.status)
    , facilityOptions(other.facilityOptions)
    , life_quality_score(other.life_quality_score)
    , economy_score(other.economy_score)
    , environment_score(other.environment_score) {
    
    // Deep copy facilities
    for (const Facility* facility : other.facilities) {
        facilities.push_back(facility->clone());
    }

    for (const Facility* facility : other.underConstruction) {
        underConstruction.push_back(facility->clone());
    }
}

/*Plan& Plan::operator=(const Plan& other) {
    if (this != &other) {
        // Clean up current resources
        if (selectionPolicy) {
            delete selectionPolicy;
        }
        for (Facility* facility : facilities) {
            delete facility;
        }
        facilities.clear();

        for (Facility* facility : underConstruction) {
            delete facility;
        }
        underConstruction.clear();

        // Copy data from `other`
        plan_id = other.plan_id;
        settlement = other.settlement;
        selectionPolicy = other.selectionPolicy ? other.selectionPolicy->clone() : nullptr;
        status = other.status;
        facilityOptions = other.facilityOptions;
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;

        // Deep copy facilities
        for (const Facility* facility : other.facilities) {
            facilities.push_back(new Facility(*facility));
        }

        for (const Facility* facility : other.underConstruction) {
            underConstruction.push_back(new Facility(*facility));
        }
    }
    return *this;
}
*/


Plan::Plan(Plan&& other) noexcept 
    : plan_id(other.plan_id)
    , settlement(other.settlement)
    , selectionPolicy(other.selectionPolicy)
    , status(other.status)
    , facilities(std::move(other.facilities))
    , underConstruction(std::move(other.underConstruction))
    , facilityOptions(std::move(other.facilityOptions))
    , life_quality_score(other.life_quality_score)
    , economy_score(other.economy_score)
    , environment_score(other.environment_score) {
    
    // Nullify moved-from object's pointers
    other.selectionPolicy = nullptr;
    other.settlement = nullptr;
}

/*
Plan& Plan::operator=(Plan&& other) noexcept {
    if (this != &other) {
        // Clean up current resources
        if (selectionPolicy) {
            delete selectionPolicy;
        }
        for (Facility* facility : facilities) {
            delete facility;
        }
        facilities.clear();

        for (Facility* facility : underConstruction) {
            delete facility;
        }
        underConstruction.clear();

        // Transfer data from `other`
        plan_id = other.plan_id;
        settlement = other.settlement;
        selectionPolicy = other.selectionPolicy;
        status = other.status;
        facilities = std::move(other.facilities);
        underConstruction = std::move(other.underConstruction);

        facilityOptions = std::move(other.facilityOptions);
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;

        // Nullify moved-from object's pointers
        other.selectionPolicy = nullptr;
        other.settlement = nullptr;
    }
    return *this;
    }
*/
