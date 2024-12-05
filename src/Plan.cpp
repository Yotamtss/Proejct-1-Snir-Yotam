#include "Plan.h"
#include <iostream>
#include "SelectionPolicy.h"
#include <sstream>

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


//DEEP COPY CONSTRUCTOR

Plan::Plan(const int planId,
           const Settlement &settlement,
           SelectionPolicy *selectionPolicy,
           const std::vector<FacilityType> &facilityOptions,
           int life_quality_score,
           int economy_score,
           int environment_score,
           std::vector<Facility *> facilities,
           std::vector<Facility *> underConstruction)
    : Plan(planId, settlement, selectionPolicy, facilityOptions) // Delegate to first constructor
{
    // Additional initialization
    this->life_quality_score = life_quality_score;
    this->economy_score = economy_score;
    this->environment_score = environment_score;
    this->facilities = std::move(facilities);
    this->underConstruction = std::move(underConstruction);
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
    while (underConstruction.size() < static_cast<size_t>(settlement.getConstructionLimit()) && status != PlanStatus::BUSY) {
            // Select facility based on current policy
            Facility* newFacility = new Facility(selectionPolicy->selectFacility(facilityOptions), settlement.getName());
            addFacility(newFacility);
        }
    }

    // Stage 3: Process facilities under construction
    vector<Facility*> completedFacilities;
    for (int i = underConstruction.size() - 1; i >= 0; i--)
    {
        FacilityStatus facilityStatus = underConstruction[i]->step();
        if (facilityStatus == FacilityStatus::OPERATIONAL) {
            // Update scores
            addFacility(underConstruction[i]);
            underConstruction.erase(underConstruction.begin() + i);
    }

    }

    // Stage 4: Update plan status
    status = (underConstruction.size() >= static_cast<size_t>(settlement.getConstructionLimit()))
        ? PlanStatus::BUSY 
        : PlanStatus::AVALIABLE;
}

void Plan::printStatus(){

    std::cout << toString();
}

const vector<Facility*> &Plan::getFacilities() const {
    return facilities;
}

const int Plan::getID() const {
    return plan_id;
}

void Plan::addFacility(Facility* facility) {
    // Update scores

    if (facility->getStatus() == FacilityStatus::OPERATIONAL)
    {
        life_quality_score += facility->getLifeQualityScore();
        economy_score += facility->getEconomyScore();
        environment_score += facility->getEnvironmentScore();
        facilities.push_back(facility);
    }
    else
    {
        underConstruction.push_back(facility);
    }

}


const string Plan::toString() const {
    string result = "planID: " + std::to_string(plan_id) + " settlementName: " + settlement.getName() + "\n";
    result += "planStatus: " + string(status == PlanStatus::BUSY ? "BUSY" : "AVALIABLE") + "\n";
    
    // Selection policy string representation
    string policyStr;
    policyStr = selectionPolicy->toString();
    if (policyStr == "NaiveSelection") policyStr = "nve";
    else if (policyStr == "BalancedSelection") policyStr = "bal";
    else if (policyStr == "EconomySelection") policyStr = "eco";
    else if (policyStr == "SustainabilitySelection") policyStr = "env";
    
    result += "selectionPolicy: " + policyStr + "\n";
    result += "LifeQualityScore: " + std::to_string(life_quality_score) + "\n";
    result += "EconomyScore: " + std::to_string(economy_score) + "\n";
    result += "EnvironmentScore: " + std::to_string(environment_score) + "\n";

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
    , facilities()
    , underConstruction()
    , facilityOptions(other.facilityOptions)
    , life_quality_score(other.life_quality_score)
    , economy_score(other.economy_score)
    , environment_score(other.environment_score)
     {
    
    // Deep copy facilities
    for (const Facility* facility : other.facilities) {
        facilities.push_back(facility->clone());
    }

    for (const Facility* facility : other.underConstruction) {
        underConstruction.push_back(facility->clone());
    }
}


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
    , environment_score(other.environment_score)
     {
    
    // Nullify moved-from object's pointers
    other.selectionPolicy = nullptr;
    //other.settlement = nullptr;
}

const vector<Facility*> &Plan::getConstruction() const {
    return underConstruction;
}

const string Plan::getSelectionPolicy() const
{
    string str = selectionPolicy->toString();

    if(str == "NaiveSelection")
        return "nve";
    else if(str == "BalancedSelection")
        return "bal";
    else if (str == "EconomySelection")
        return "eco";
    else if (str == "SustainabilitySelection")
        return "env";
    return str; 
}

//RABIN SHIT


SelectionPolicy *Plan::getPolicy() const
{
    return selectionPolicy;
}

const string &Plan::getSettlement() const
{
    return settlement.getName();
}


