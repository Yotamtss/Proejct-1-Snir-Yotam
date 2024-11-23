#include "SelectionPolicy.h"
#include <stdexcept>
#include <limits>
#include <sstream>

// ----------------------------------------
// Base Class: SelectionPolicy
// ----------------------------------------
SelectionPolicy::~SelectionPolicy() = default;

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

    for (size_t i = 0; i < facilitiesOptions.size(); ++i) {
        const FacilityType& facility = facilitiesOptions[i];

        // Calculate balance as the sum of differences between scores
        double balance = std::abs(LifeQualityScore - facility.getLifeQualityScore()) +
                         std::abs(EconomyScore - facility.getEconomyScore()) +
                         std::abs(EnvironmentScore - facility.getEnvironmentScore());

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

    // Select the facility with the highest economy score
    size_t bestIndex = 0;
    int bestEconomyScore = facilitiesOptions[0].getEconomyScore();

    for (size_t i = 1; i < facilitiesOptions.size(); ++i) {
        if (facilitiesOptions[i].getEconomyScore() > bestEconomyScore) {
            bestEconomyScore = facilitiesOptions[i].getEconomyScore();
            bestIndex = i;
        }
    }

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

    // Select the facility with the highest environment score
    size_t bestIndex = 0;
    int bestEnvironmentScore = facilitiesOptions[0].getEnvironmentScore();

    for (size_t i = 1; i < facilitiesOptions.size(); ++i) {
        if (facilitiesOptions[i].getEnvironmentScore() > bestEnvironmentScore) {
            bestEnvironmentScore = facilitiesOptions[i].getEnvironmentScore();
            bestIndex = i;
        }
    }

    return facilitiesOptions[bestIndex];
}

const string SustainabilitySelection::toString() const {
    return "SustainabilitySelection";
}

SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this); // Copy constructor for cloning
}
