#include "RidaGOAPAIController.h"
#include "GOAPFact.h"
#include "GOAPAgentComponent.h"
#include "GOAPMotive.h"

ARidaGOAPAIController::ARidaGOAPAIController()
{
    PrimaryActorTick.bCanEverTick = false;
}

UGOAPAgentComponent* ARidaGOAPAIController::GetGOAPComponent() const
{
    if (const APawn* ControlledPawn = GetPawn())
        return ControlledPawn->FindComponentByClass<UGOAPAgentComponent>();
    return nullptr;
}

void ARidaGOAPAIController::RequestGoal(TSubclassOf<UGOAPMotive> MotiveClass, const TArray<FGOAPFact>& SeedFacts)
{
    UGOAPAgentComponent* GOAP = GetGOAPComponent();
    if (!GOAP || !MotiveClass) return;

    GOAP->ClearMotives();

    for (const FGOAPFact& Fact : SeedFacts)
    {
        switch (Fact.Type)
        {
            case EGOAPGoalType::Bool:
                GOAP->SetWorldBool (Fact.Key, Fact.BoolValue);
                break;
            case EGOAPGoalType::Int:
                GOAP->SetWorldInt  (Fact.Key, Fact.IntValue);
                break;
            case EGOAPGoalType::Float:
                GOAP->SetWorldFloat(Fact.Key, Fact.FloatValue);
                break;
        }
    }

    SenseWorldState();
    GOAP->AddMotive(MotiveClass);
}

void ARidaGOAPAIController::InjectWorldBool(const FString& Key, bool Value)
{
    if (UGOAPAgentComponent* GOAP = GetGOAPComponent())
    {
        GOAP->SetWorldBool(Key, Value);
    }
}

void ARidaGOAPAIController::InjectWorldInt(const FString& Key, int32 Value)
{
    if (UGOAPAgentComponent* GOAP = GetGOAPComponent())
    {
        GOAP->SetWorldInt(Key, Value);
    }
}

void ARidaGOAPAIController::InjectWorldFloat(const FString& Key, float Value)
{
    if (UGOAPAgentComponent* GOAP = GetGOAPComponent())
    {
        GOAP->SetWorldFloat(Key, Value);
    }
}