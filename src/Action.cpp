#include <iostream>
#include <string>
#include <vector>
#include "Simulation.h"

using namespace std;

// Forward declarations for enums
enum class SettlementType;
enum class FacilityCategory;

enum class ActionStatus {
    COMPLETED, ERROR
};

// BaseAction Implementation
class BaseAction {
public:
    BaseAction() : status(ActionStatus::COMPLETED), errorMsg("") {}

    ActionStatus getStatus() const {
        return status;
    }

    virtual void act(Simulation &simulation) = 0;

    virtual const string toString() const = 0;

    virtual BaseAction* clone() const = 0;

    virtual ~BaseAction() = default;

protected:
    void complete() {
        status = ActionStatus::COMPLETED;
    }

    void error(string errorMsg) {
        status = ActionStatus::ERROR;
        this->errorMsg = errorMsg;
        cout << "Error: " << errorMsg << endl;
    }

    const string &getErrorMsg() const {
        return errorMsg;
    }

private:
    string errorMsg;
    ActionStatus status;
};

// SimulateStep Implementation
class SimulateStep : public BaseAction {
public:
    SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

    void act(Simulation &simulation) override {
        // Simulation logic to step forward
        for (int i = 0; i < numOfSteps; ++i) {
            simulation.step();
        }
        complete();
    }

    const string toString() const override {
        return "SimulateStep " + to_string(numOfSteps);
    }

    SimulateStep* clone() const override {
        return new SimulateStep(*this);
    }

private:
    const int numOfSteps;
};

// AddPlan Implementation
class AddPlan : public BaseAction {
public:
    AddPlan(const string &settlementName, const string &selectionPolicy)
        : settlementName(settlementName), selectionPolicy(selectionPolicy) {}

    void act(Simulation &simulation) override {
        if (!simulation.addPlan(settlementName, selectionPolicy)) {
            error("Cannot create this plan");
        } else {
            complete();
        }
    }

    const string toString() const override {
        return "AddPlan " + settlementName + " " + selectionPolicy;
    }

    AddPlan* clone() const override {
        return new AddPlan(*this);
    }

private:
    const string settlementName;
    const string selectionPolicy;
};

// AddSettlement Implementation
class AddSettlement : public BaseAction {
public:
    AddSettlement(const string &settlementName, SettlementType settlementType)
        : settlementName(settlementName), settlementType(settlementType) {}

    void act(Simulation &simulation) override {
        if (!simulation.addSettlement(settlementName, settlementType)) {
            error("Settlement already exists");
        } else {
            complete();
        }
    }

    const string toString() const override {
        return "AddSettlement " + settlementName;
    }

    AddSettlement* clone() const override {
        return new AddSettlement(*this);
    }

private:
    const string settlementName;
    const SettlementType settlementType;
};

// AddFacility Implementation
class AddFacility : public BaseAction {
public:
    AddFacility(const string &facilityName, FacilityCategory facilityCategory, int price,
                int lifeQualityScore, int economyScore, int environmentScore)
        : facilityName(facilityName), facilityCategory(facilityCategory), price(price),
          lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}

    void act(Simulation &simulation) override {
        if (!simulation.addFacility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore)) {
            error("Facility already exists");
        } else {
            complete();
        }
    }

    const string toString() const override {
        return "AddFacility " + facilityName;
    }

    AddFacility* clone() const override {
        return new AddFacility(*this);
    }

private:
    const string facilityName;
    const FacilityCategory facilityCategory;
    const int price;
    const int lifeQualityScore;
    const int economyScore;
    const int environmentScore;
};

// Remaining classes follow a similar pattern
// ...

// Example for another class: PrintActionsLog
class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog() {}

    void act(Simulation &simulation) override {
        simulation.printActionsLog();
        complete();
    }

    const string toString() const override {
        return "PrintActionsLog";
    }

    PrintActionsLog* clone() const override {
        return new PrintActionsLog(*this);
    }
    // PrintPlanStatus Implementation
class PrintPlanStatus : public BaseAction {
public:
    PrintPlanStatus(int planId) : planId(planId) {}

    void act(Simulation &simulation) override {
        if (!simulation.printPlanStatus(planId)) {
            error("Plan doesn't exist");
        } else {
            complete();
        }
    }

    const string toString() const override {
        return "PrintPlanStatus " + to_string(planId);
    }

    PrintPlanStatus* clone() const override {
        return new PrintPlanStatus(*this);
    }

private:
    const int planId;
};

// ChangePlanPolicy Implementation
class ChangePlanPolicy : public BaseAction {
public:
    ChangePlanPolicy(const int planId, const string &newPolicy)
        : planId(planId), newPolicy(newPolicy) {}

    void act(Simulation &simulation) override {
        if (!simulation.changePlanPolicy(planId, newPolicy)) {
            error("Cannot change selection policy");
        } else {
            complete();
        }
    }

    const string toString() const override {
        return "ChangePlanPolicy " + to_string(planId) + " " + newPolicy;
    }

    ChangePlanPolicy* clone() const override {
        return new ChangePlanPolicy(*this);
    }

private:
    const int planId;
    const string newPolicy;
};

// PrintActionsLog Implementation
class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog() {}

    void act(Simulation &simulation) override {
        simulation.printActionsLog();
        complete();
    }

    const string toString() const override {
        return "PrintActionsLog";
    }

    PrintActionsLog* clone() const override {
        return new PrintActionsLog(*this);
    }
};

// Close Implementation
class Close : public BaseAction {
public:
    Close() {}

    void act(Simulation &simulation) override {
        simulation.closeSimulation();
        complete();
    }

    const string toString() const override {
        return "Close";
    }

    Close* clone() const override {
        return new Close(*this);
    }
};

// BackupSimulation Implementation
class BackupSimulation : public BaseAction {
public:
    BackupSimulation() {}

    void act(Simulation &simulation) override {
        simulation.backup();
        complete();
    }

    const string toString() const override {
        return "BackupSimulation";
    }

    BackupSimulation* clone() const override {
        return new BackupSimulation(*this);
    }
};

// RestoreSimulation Implementation
class RestoreSimulation : public BaseAction {
public:
    RestoreSimulation() {}

    void act(Simulation &simulation) override {
        if (!simulation.restore()) {
            error("No backup available");
        } else {
            complete();
        }
    }

    const string toString() const override {
        return "RestoreSimulation";
    }

    RestoreSimulation* clone() const override {
        return new RestoreSimulation(*this);
    }
};

