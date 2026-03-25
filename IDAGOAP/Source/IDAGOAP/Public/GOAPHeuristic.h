#pragma once

#include <memory>
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "heuristic.h"
#include "GOAPHeuristic.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class IDAGOAP_API UGOAPHeuristic : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, Category = "GOAP")
    int32 Estimate(const TMap<FString, bool>& CurrentBools, const TMap<FString, bool>& GoalBools);
    virtual int32 Estimate_Implementation(const TMap<FString, bool>& CurrentBools, const TMap<FString, bool>& GoalBools)
    {
        int32 Count = 0;
        for (auto& [Key, GoalVal] : GoalBools)
        {
            const bool* Current = CurrentBools.Find(Key);
            if (!Current || *Current != GoalVal) ++Count;
        }
        return Count;
    }

    std::shared_ptr<rida_goap::heuristic> BuildBridge();
};
