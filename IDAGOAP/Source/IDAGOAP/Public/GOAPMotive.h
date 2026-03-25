#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "motive.h"
#include "GOAPMotive.generated.h"

UCLASS(Blueprintable, BlueprintType)
class IDAGOAP_API UGOAPMotive : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GOAP")
    FString MotiveName = TEXT("Unnamed Motive");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GOAP")
    int32 Priority = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GOAP")
    bool bPersistent = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GOAP")
    TMap<FString, bool> GoalBools;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GOAP")
    TMap<FString, int32> GoalInts;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GOAP")
    TMap<FString, float> GoalFloats;
};
