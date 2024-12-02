#include <iostream>
#include <string>
#include <vector>
#include "Simulation.h"
#include "SelectionPolicy.h"
#include "Action.h"

using namespace std;

// BaseAction Implementation

BaseAction::BaseAction() : status(ActionStatus::COMPLETED), errorMsg("") {}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    cout << "Error: " << errorMsg << endl;
}

const string &BaseAction::getErrorMsg() const
{
    return errorMsg;
}


// SimulateStep Implementation
    SimulateStep::SimulateStep(const int numOfSteps) : BaseAction(), numOfSteps(numOfSteps) {}

    void SimulateStep::act(Simulation &simulation)
    {
        // Simulation logic to step forward
        for (int i = 0; i < numOfSteps; ++i)
        {
            simulation.step();
        }
        complete();
    }

    const string SimulateStep::toString() const 
    {
        return "SimulateStep " + to_string(numOfSteps);
    }

    SimulateStep *SimulateStep::clone() const
    {
        return new SimulateStep(*this);
    }


// AddPlan Implementation
    AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy)
        : settlementName(settlementName), selectionPolicy(selectionPolicy) {}

    void AddPlan::act(Simulation &simulation)
    {
        // Check if the settlement exists and if the policy is valid
        if (!simulation.isSettlementExists(settlementName))
        {
            error("Cannot create this plan: Settlement does not exist.");
            return;
        }

        if (selectionPolicy != "env" && selectionPolicy != "eco" &&
            selectionPolicy != "bal" && selectionPolicy != "nve")
        {
            error("Cannot create this plan: Invalid selection policy.");
            return;
        }

        // Create the appropriate selection policy
        SelectionPolicy *policy = nullptr;
        if (selectionPolicy == "nve")
        {
            policy = new NaiveSelection();
        }
        else if (selectionPolicy == "bal")
        {
            policy = new BalancedSelection(0, 0, 0);
        }
        else if (selectionPolicy == "eco")
        {
            policy = new EconomySelection();
        }
        else if (selectionPolicy == "env")
        {
            policy = new SustainabilitySelection();
        }
        else{
            error("Couldn't access any type");
        }

        // Add the plan to the simulation
        if (policy)
        {
            simulation.addPlan(simulation.getSettlement(settlementName), policy);
            for(Plan plan : simulation.plans)
                {}
                //std::cout << plan.toString() << "\n";
            //delete policy; // Clean up memory after usage
        }
        else
        {
            error("Cannot create this plan: Unknown error.");
        }
    }
    const string AddPlan::toString() const
    {
        return "AddPlan " + settlementName + " " + selectionPolicy;
    }

    AddPlan *AddPlan::clone() const 
    {
        return new AddPlan(*this);
    }

// AddSettlement Implementation

    AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType)
        : settlementName(settlementName), settlementType(settlementType) {}

    void AddSettlement::act(Simulation &simulation) 
    {
        Settlement *sttl = new Settlement(settlementName, settlementType);
        if (simulation.addSettlement(sttl))
        {
            complete();

        }
        else
        {
            error("Settlement already exists");
        }
    }

    const string AddSettlement::toString() const 
    {
        return "AddSettlement " + settlementName;
    }

    AddSettlement *AddSettlement::clone() const 
    {
        return new AddSettlement(*this);
    }

// AddFacility Implementation
    AddFacility::AddFacility(const string &facilityName, FacilityCategory facilityCategory, int price,
                             int lifeQualityScore, int economyScore, int environmentScore)
        : BaseAction(), facilityName(facilityName), facilityCategory(facilityCategory), price(price),
          lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}

    void AddFacility::act(Simulation &simulation)
    {
        FacilityType fac = FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
        if (!simulation.addFacility(fac))
        {
            error("Facility already exists");
        }
        else
        {
            complete();
        }
    }

    const string AddFacility::toString() const
    {
        return "AddFacility " + facilityName;
    }

    AddFacility *AddFacility::clone() const
    {
        return new AddFacility(*this);
    }

    PrintPlanStatus::PrintPlanStatus(int planId) : BaseAction(), planId(planId) {}
    void PrintPlanStatus::act(Simulation &simulation)
    {
        simulation.getPlan(planId).printStatus();
    }
    PrintPlanStatus *PrintPlanStatus::clone() const
    {
        return new PrintPlanStatus(*this);
    }
    const string PrintPlanStatus::toString() const
    {
        return "Plan status of " + planId;
    }

    ChangePlanPolicy::ChangePlanPolicy(const int planId, const std::string &newPolicy) : BaseAction(), planId(planId), newPolicy(newPolicy) {}
    void ChangePlanPolicy::act(Simulation &simulation)
    {
        if (newPolicy == "nve")
        {
            NaiveSelection *ns = new NaiveSelection();
            simulation.getPlan(planId).setSelectionPolicy(ns);
        }
        else if (newPolicy == "bal")
        {
            Plan currPlan = simulation.getPlan(planId);
            int lif_score_tmp = currPlan.getlifeQualityScore(); 
            int env_score_tmp = currPlan.getEnvironmentScore(); 
            int eco_score_tmp = currPlan.getEconomyScore(); 
            for(Facility *facil : simulation.getPlan(planId).getConstruction())
            {
                lif_score_tmp += facil->getLifeQualityScore();
                env_score_tmp += facil->getEnvironmentScore();
                eco_score_tmp += facil->getEconomyScore();
            }
            BalancedSelection *bs = new BalancedSelection(lif_score_tmp, eco_score_tmp, env_score_tmp);
            simulation.getPlan(planId).setSelectionPolicy(bs);
        }
        else if (newPolicy == "eco")
        {
            EconomySelection *es = new EconomySelection();
            simulation.getPlan(planId).setSelectionPolicy(es);
        }
        else if (newPolicy == "env")
        {
            SustainabilitySelection *ss = new SustainabilitySelection();
            simulation.getPlan(planId).setSelectionPolicy(ss);
        }
    }
    ChangePlanPolicy *ChangePlanPolicy::clone() const
    {
        return new ChangePlanPolicy(*this);
    }
    const string ChangePlanPolicy::toString() const
    {
        return "Plan status of " + to_string(planId) + "changed to" + newPolicy;
    }

// PrintActionsLog Implementation

    PrintActionsLog::PrintActionsLog() : BaseAction() {}

    void PrintActionsLog::act(Simulation &simulation)
    {

        simulation.printLog();

        // vector<BaseAction *> actionsLog = simulation.getActionsLog();
        // for (const auto *action : actionsLog)
        // {
        //     if (action != this)
        //     {
        //         std::cout << action->toString() << " ";
        //         switch (action->getStatus())
        //         {
        //         case ActionStatus::COMPLETED:
        //             std::cout << "COMPLETED";
        //             break;
        //         case ActionStatus::ERROR:
        //             std::cout << "ERROR";
        //             break;
        //         }
        //         std::cout << std::endl; // Move to the next line
        //     }
        // }
        complete();
    }

    const string PrintActionsLog::toString() const
    {
        return "PrintActionsLog";
    }

    PrintActionsLog *PrintActionsLog::clone() const
    {
        return new PrintActionsLog(*this);
    }


// Close Implementation
    Close::Close() : BaseAction() {}

    void Close::act(Simulation &simulation)
    {
        simulation.close();
        complete();
    }

    const string Close::toString() const
    {
        return "Close";
    }

    Close *Close::clone() const
    {
        return new Close(*this);
    }



// // BackupSimulation Implementation
//     BackupSimulation::BackupSimulation() : BaseAction() {}

//     void BackupSimulation::act(Simulation &simulation)
//     {
//         if (backup)
//         {
//             delete backup;
//         }
//         backup = new Simulation(simulation);
//         complete();
//     }

//     const string BackupSimulation::toString() const
//     {
//         return "BackupSimulation";
//     }

//     BackupSimulation *BackupSimulation::clone() const
//     {
//         return new BackupSimulation(*this);
//     }


// // RestoreSimulation Implementation
//     RestoreSimulation::RestoreSimulation() : BaseAction() {}

//     void RestoreSimulation::act(Simulation &simulation)
//     {
//         if (backup == nullptr)
//         {
//             error("No backup available");
//             return;
//         }
//         simulation = *backup;
//         complete();
//         simulation.addAction(this);
//     }
//     const string RestoreSimulation::toString() const
//     {
//         return "RestoreSimulation";
//     }

//     RestoreSimulation *RestoreSimulation::clone() const
//     {
//         return new RestoreSimulation(*this);
//     }
