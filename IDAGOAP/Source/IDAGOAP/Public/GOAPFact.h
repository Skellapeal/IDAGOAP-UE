#pragma once
#include "CoreMinimal.h"
#include "GOAPFact.generated.h"

UENUM(BlueprintType)
enum class EGOAPGoalType : uint8
{
    Bool,
    Int,
    Float
};

USTRUCT(BlueprintType)
struct IDAGOAP_API FGOAPFact
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Key;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EGOAPGoalType Type = EGOAPGoalType::Bool;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool BoolValue = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 IntValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FloatValue = 0.f;
};