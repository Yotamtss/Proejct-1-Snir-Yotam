#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
        const int getID() const;
        Plan(const Plan& other);                          // Copy constructor
        Plan& operator=(const Plan& other) = delete;               // Copy assignment operator
        Plan(Plan&& other) noexcept;                      // Move constructor
        Plan& operator=(Plan&& other) noexcept = delete;           // Move assignment operator
        ~Plan();   
        const vector<Facility*> &getConstruction() const;
        const string getSelectionPolicy() const;

        
    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
        int tmp_life_qua_score, tmp_eco_score, tmp_env_score;
};