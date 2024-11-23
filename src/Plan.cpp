#include "Plan.h"

// Constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
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
        while (underConstruction.size() < settlement.getConstructionLimit() && status != PlanStatus::BUSY) {
            // Select facility based on current policy
            Facility* newFacility = new Facility(selectionPolicy->selectFacility(facilityOptions), settlement.getName());
            underConstruction.push_back(newFacility);
        }
    }

    // Stage 3: Process facilities under construction
    vector<Facility*> completedFacilities;
    for (Facility* facility : underConstruction) {
        FacilityStatus facilityStatus = facility->step();
        if (facilityStatus == FacilityStatus::OPERATIONAL) {
            // Update scores
            life_quality_score += facility->getLifeQualityScore();
            economy_score += facility->getEconomyScore();
            environment_score += facility->getEnvironmentScore();
            
            // Move to completed facilities
            facilities.push_back(facility);
            completedFacilities.push_back(facility);
        }
    }

    // Remove completed facilities from underConstruction
    for (Facility* completed : completedFacilities) {
        underConstruction.erase(
            std::remove(underConstruction.begin(), underConstruction.end(), completed),
            underConstruction.end()
        );
    }

    // Stage 4: Update plan status
    status = (underConstruction.size() >= settlement.getConstructionLimit()) 
        ? PlanStatus::BUSY 
        : PlanStatus::AVALIABLE;
}

void Plan::printStatus() {
    std::cout << toString();
}

const vector<Facility*> &Plan::getFacilities() const {
    return facilities;
}

void Plan::addFacility(Facility* facility) {
    facilities.push_back(facility);
    // Update scores
    life_quality_score += facility->getLifeQualityScore();
    economy_score += facility->getEconomyScore();
    environment_score += facility->getEnvironmentScore();
}

const string Plan::toString() const {
    string result = "planID: " + std::to_string(plan_id) + " settlementName: " + settlement.getName() + "\n";
    result += "planStatus: " + string(status == PlanStatus::BUSY ? "BUSY" : "AVALIABLE") + "\n";
    
    // Selection policy string representation
    string policyStr;
    if (dynamic_cast<NaiveSelectionPolicy*>(selectionPolicy)) policyStr = "nve";
    else if (dynamic_cast<BalancedSelectionPolicy*>(selectionPolicy)) policyStr = "bal";
    else if (dynamic_cast<EconomySelectionPolicy*>(selectionPolicy)) policyStr = "eco";
    else if (dynamic_cast<SustainabilitySelectionPolicy*>(selectionPolicy)) policyStr = "env";
    
    result += "selectionPolicy: " + policyStr + "\n";
    result += "LifeQualityScore: " + std::to_string(life_quality_score) + "\n";
    result += "EconomyScore: " + std::to_string(economy_score) + "\n";
    result += "EnvironmentScore: " + std::to_string(environment_score) + "\n";

    // Sort facilities by ID before printing
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

    return result;
}