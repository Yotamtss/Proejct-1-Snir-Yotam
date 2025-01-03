#include "Facility.h"
#include <iostream>

// Constructor implementation using initialization list for const members
FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price,
                           const int lifeQuality_score, const int economy_score, const int environment_score)
    : name(name) // Initialize const string name
      ,
      category(category) // Initialize const FacilityCategory
      ,
      price(price) // Initialize const int price
      ,
      lifeQuality_score(lifeQuality_score) // Initialize const int lifeQuality_score
      ,
      economy_score(economy_score) // Initialize const int economy_score
      ,
      environment_score(environment_score) // Initialize const int environment_score
{
}


// Getter implementations
const string &FacilityType::getName() const
{
    return name;
}

int FacilityType::getCost() const
{
    return price;
}

int FacilityType::getLifeQualityScore() const
{
    return lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const
{
    return environment_score;
}

int FacilityType::getEconomyScore() const
{
    return economy_score;
}

FacilityCategory FacilityType::getCategory() const
{
    return category;
}

// Destructor: does nothing since FacilityType has no dynamic memory
FacilityType::~FacilityType() 
{
}

Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category,
                   const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score) // Call base constructor
      ,
      settlementName(settlementName) // Initialize const member
      ,
      status(FacilityStatus::UNDER_CONSTRUCTIONS) // Initialize with default status
      ,
      timeLeft(price) // Initialize timeLeft with price (construction time)
{
}

Facility::Facility(const FacilityType &type, const string &settlementName)
    : FacilityType(type.getName(), type.getCategory(), type.getCost(), type.getLifeQualityScore(), type.getEconomyScore(), type.getEnvironmentScore()), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price)
{
}

// Getter for settlement name
const string &Facility::getSettlementName() const
{
    return settlementName;
}

// Getter for time left
const int Facility::getTimeLeft() const
{
    return timeLeft;
}

// Implementation of step() method (added)
FacilityStatus Facility::step()
{
    if (status == FacilityStatus::UNDER_CONSTRUCTIONS)
    {
        if (timeLeft > 0)
        {
            timeLeft--;
            if (timeLeft == 0)
            {
                status = FacilityStatus::OPERATIONAL;
            }
        }
    }
    return status;
}

// Status setter
void Facility::setStatus(FacilityStatus status)
{
    this->status = status;
}

// Status getter
const FacilityStatus &Facility::getStatus() const
{
    return status;
}

FacilityType *FacilityType::clone() const
{
    return new FacilityType(*this);
}

Facility *Facility::clone() const
{
    return new Facility(*this);
}

// Destructor: does nothing since Facility has no dynamic memory
Facility::~Facility() 
{
}

// String representation
const string Facility::toString() const
{
    string statusStr;  
    if(status == FacilityStatus::UNDER_CONSTRUCTIONS)
        statusStr = "UNDER_CONSTRUCTIONS";
    else
        statusStr = "OPERATIONAL";   

    return "facilityName: " + getName()+ "\n" + "facilityStatus: " + statusStr;
}
