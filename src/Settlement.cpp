#include "Settlement.h"
#include <sstream>

Settlement::Settlement(const string &name, SettlementType type) : name(name), type(type) {}


Settlement::Settlement(const Settlement* other)
 : name((*other).getName())
 , type((*other).getType()) {}


/*

Settlement& Settlement::operator=(const Settlement& other) {
    if (this != &other) {
        const_cast<string&>(name) = other.name;
        type = other.type;
    }
    return *this;
}

Settlement::Settlement(Settlement&& other) noexcept : name(std::move(other.name)), type(other.type) {}

Settlement& Settlement::operator=(Settlement&& other) noexcept {
    if (this != &other) {
        const_cast<string&>(name) = std::move(other.name);
        type = other.type;
    }
    return *this;
}

Settlement::~Settlement() = default;

*/

const string &Settlement::getName() const {
    return name;
}

SettlementType Settlement::getType() const {
    return type;
}

int Settlement::getConstructionLimit() const{
    if(type == SettlementType::VILLAGE)
        return 1;
    else if(type == SettlementType::CITY)
        return 2;
    else
        return 3;
}

const string Settlement::toString() const {
    std::ostringstream oss;
    oss << "Settlement Name: " << name << ", Type: ";
    switch (type) {
        case SettlementType::VILLAGE:
            oss << "Village";
            break;
        case SettlementType::CITY:
            oss << "City";
            break;
        case SettlementType::METROPOLIS:
            oss << "Metropolis";
            break;
    }
    return oss.str();
}
