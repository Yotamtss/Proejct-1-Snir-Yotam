#include "Facility.h"
#include <cassert>


// Basic unit tests
int main() {
    // Test basic facility creation
    FacilityType facility("School", FacilityCategory::ECONOMY, 5, 3, 2, 1);
    assert(facility.getName() == "School");
    assert(facility.getCost() == 5);
    
    // Test scores calculation
    assert(facility.getLifeQualityScore() == 3);
    assert(facility.getEconomyScore() == 2);
    assert(facility.getEnvironmentScore() == 1);
    
    // Test Rule of 5
    FacilityType copyFacility = facility;  // Copy constructor
    assert(copyFacility.getName() == facility.getName());
}