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
    vector<int> new_vals = {LifeQualityScore, EconomyScore, EnvironmentScore};
    
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
    LifeQualityScore += facilitiesOptions[bestIndex].getLifeQualityScore();
    EconomyScore += facilitiesOptions[bestIndex].getEconomyScore();
    EnvironmentScore += facilitiesOptions[bestIndex].getEnvironmentScore();

    return facilitiesOptions[bestIndex];
}

const string BalancedSelection::toString() const {
    return "BalancedSelection";

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
    int index = lastSelectedIndex;
    while (!found) {
        index = (index + 1) % facilitiesOptions.size();
        FacilityType curr = facilitiesOptions[index];
        found = (curr.getCategory() == FacilityCategory::ECONOMY);
    }
    lastSelectedIndex = index;
    return facilitiesOptions[index];
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
    int index = lastSelectedIndex;
    while (!found) {
        index = (index + 1) % facilitiesOptions.size();
        FacilityType curr = facilitiesOptions[index];
        found = (curr.getCategory() == FacilityCategory::ENVIRONMENT);
    }
    lastSelectedIndex = index;
    return facilitiesOptions[index];
}

const string SustainabilitySelection::toString() const {
    return "SustainabilitySelection";
}

SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this); // Copy constructor for cloning
}
