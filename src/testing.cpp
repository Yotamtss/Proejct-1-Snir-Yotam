#include "Facility.h"
#include "Settlement.h"
#include "Plan.h"
#include <cassert>
#include <iostream>
#include <vector>

// Basic unit tests
int main() {
    Facility school("School", "TestTown", FacilityCategory::LIFE_QUALITY, 5, 3, 2, 1);
    assert(school.getName() == "School");
    assert(school.getSettlementName() == "TestTown");
    assert(school.getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS);
    assert(school.getTimeLeft() == 5);
    
    // Test second constructor
    FacilityType marketType("Market", FacilityCategory::ECONOMY, 3, 1, 4, 1);
    Facility market(marketType, "TestTown");
    assert(market.getName() == "Market");
    assert(market.getTimeLeft() == 3);
    
    // Test step function
    FacilityStatus status = market.step();
    assert(market.getTimeLeft() == 2);
    assert(status == FacilityStatus::UNDER_CONSTRUCTIONS);
    
    // Test completion
    market.step();
    market.step();
    status = market.step();
    assert(status == FacilityStatus::OPERATIONAL);

//Settlements tests

    Settlement settlement("TestCity", SettlementType:: CITY);
    
    assert(settlement.getName() == "TestCity");  // City limit
    assert(settlement.getType() == SettlementType::CITY);

//Selection Policy tests

   vector<FacilityType> facilities = {
        FacilityType("School", FacilityCategory::LIFE_QUALITY, 5, 3, 2, 1),
        FacilityType("Market", FacilityCategory::ECONOMY, 3, 0, 4, 1),
        FacilityType("Park", FacilityCategory::ENVIRONMENT, 4, 2, 1, 4)
    };
    
    // Test Naive Selection
    NaiveSelection naive = NaiveSelection();

     // Cyclic selection test
    const FacilityType& firstSelection = naive.selectFacility(facilities);
    assert(firstSelection.getName() == "School");  // First selection is the first facility
    const FacilityType& secondSelection = naive.selectFacility(facilities);
    assert(secondSelection.getName() == "Market");  // Second selection is the second facility
    const FacilityType& thirdSelection = naive.selectFacility(facilities);
    assert(thirdSelection.getName() == "Park");  // Third selection is the third facility
    const FacilityType& fourthSelection = naive.selectFacility(facilities);
    assert(fourthSelection.getName() == "School");  // Cycle restarts

    // ---------- Balanced Selection ----------
    BalancedSelection balanced = BalancedSelection(3, 2, 4); // Target scores: Life Quality = 3, Economy = 2, Environment = 4
    assert(balanced.toString() == "BalancedSelection(LifeQualityScore=3, EconomyScore=2, EnvironmentScore=4)");

    const FacilityType& balancedSelection = balanced.selectFacility(facilities);
    assert(balancedSelection.getName() == "School");  // Closest match to target scores
    // Change the target scores and test again
    BalancedSelection balanced2 = BalancedSelection(5, 4, 1);
    const FacilityType& balancedSelection2 = balanced2.selectFacility(facilities);
    assert(balancedSelection2.getName() == "Park");  // Closest match to updated scores

    // ---------- Economy Selection ----------
    EconomySelection economy;
    assert(economy.toString() == "EconomySelection");

    const FacilityType& economySelection = economy.selectFacility(facilities);
    assert(economySelection.getName() == "Market");  // Highest economy score

    // ---------- Sustainability Selection ----------
    SustainabilitySelection sustainability;
    assert(sustainability.toString() == "SustainabilitySelection");

    const FacilityType& sustainabilitySelection = sustainability.selectFacility(facilities);
    assert(sustainabilitySelection.getName() == "Park");  // Highest environment score

    // ---------- Edge Cases ----------
    vector<FacilityType> emptyFacilities;


//Plan tests


    //Settlement settlement("TestCity", SettlementType::CITY);
    //SelectionPolicy* policy = new NaiveSelection();
    //Plan plan(1, settlement, policy);
    
    // Test plan status
    //assert(plan.getPlanStatus() == AVAILABLE);
    
    // Test facility addition
    //plan.addFacility(/* facility parameters */);
    //assert(plan.getUnderConstructionFacilities().size() == 1);
    
    // Test step simulation
    //plan.step();
    // Verify facility construction progress

/*
Simulation sim;
    
    // Test config file parsing
    sim.parseConfigFile("test_config.txt");
    assert(sim.getSettlements().size() > 0);
    
    // Test action execution
    AddPlanAction addPlan(PARAMETERS);

    addPlan.act(sim);
    assert(sim.getPlans().size() == 1);
    
    // Test simulation steps
    SimulateStepAction step(1);
    step.act(sim);

*/

   std::cout << "Success!\n";
  


}



    






