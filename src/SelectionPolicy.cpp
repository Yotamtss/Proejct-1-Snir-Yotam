#include "SelectionPolicy.h"
#include <stdexcept>
#include <limits>
#include <sstream>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
using std::vector;

// ----------------------------------------
// Base Class: SelectionPolicy
// ----------------------------------------
//SelectionPolicy::~SelectionPolicy() = default;

// ----------------------------------------
// Derived Class: NaiveSelection
// ----------------------------------------

NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::runtime_error("No facilities available for selection.");
    }

    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size(); // Cycle through facilities
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const {
    return "NaiveSelection";
}

NaiveSelection* NaiveSelection::clone() const {
    return new NaiveSelection(*this); // Copy constructor for cloning
}

// ----------------------------------------
// Derived Class: BalancedSelection
// ----------------------------------------

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
    : LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore) {}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::runtime_error("No facilities available for selection.");
    }

    int bestIndex = 0;
    double bestBalance = std::numeric_limits<double>::max();
    vector<int> new_vals = {0, 0, 0};
    
    for (size_t i = 0; i < facilitiesOptions.size(); i++) {
        const FacilityType& facility = facilitiesOptions[i];

        new_vals[0] = facility.getLifeQualityScore() + LifeQualityScore;
        new_vals[1] = facility.getEconomyScore() + EconomyScore;
        new_vals[2] = facility.getEnvironmentScore() + EnvironmentScore;

        int balance = *max_element(new_vals.begin(), new_vals.end());
        balance -= *min_element(new_vals.begin(), new_vals.end());

        // Choose the facility with the best balance (smallest balance value)
        if (balance < bestBalance) {
            bestBalance = balance;
            bestIndex = i;
        }
    }

    return facilitiesOptions[bestIndex];
}

const string BalancedSelection::toString() const {
    std::ostringstream oss;
    oss << "BalancedSelection(LifeQualityScore=" << LifeQualityScore
        << ", EconomyScore=" << EconomyScore
        << ", EnvironmentScore=" << EnvironmentScore << ")";
    return oss.str();
}

BalancedSelection* BalancedSelection::clone() const {
    return new BalancedSelection(*this); // Copy constructor for cloning
}

// ----------------------------------------
// Derived Class: EconomySelection
// ----------------------------------------

EconomySelection::EconomySelection() : lastSelectedIndex(-1) {}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::runtime_error("No facilities available for selection.");
    }

    bool found = false;
    int bestIndex = lastSelectedIndex++;
    for (size_t i = 1; i < facilitiesOptions.size() && !found; i++) {

        FacilityType curr = facilitiesOptions[i];
        found = (curr.getCategory() == FacilityCategory::ECONOMY);
        if(found)
            {
                bestIndex = i;
            }

    }

    lastSelectedIndex = bestIndex;

    return facilitiesOptions[bestIndex];
}

const string EconomySelection::toString() const {
    return "EconomySelection";
}

EconomySelection* EconomySelection::clone() const {
    return new EconomySelection(*this); // Copy constructor for cloning
}

// ----------------------------------------
// Derived Class: SustainabilitySelection
// ----------------------------------------

SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) {}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::runtime_error("No facilities available for selection.");
    }

    bool found = false;
    int bestIndex = lastSelectedIndex++;
    for (size_t i = 1; i < facilitiesOptions.size() && !found; i++) {

        FacilityType curr = facilitiesOptions.at(i);
        found = (curr.getCategory() == FacilityCategory::ENVIRONMENT);
        if(found)
            {
                bestIndex = i;
            }

    }

    lastSelectedIndex = bestIndex;

    return facilitiesOptions[bestIndex];
}

const string SustainabilitySelection::toString() const {
    return "SustainabilitySelection";
}

SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this); // Copy constructor for cloning
}
