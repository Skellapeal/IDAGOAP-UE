#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GOAPFact.h"
#include "GOAPAgentComponent.h"
#include "GOAPMotive.h"
#include "RidaGOAPAIController.generated.h"

UCLASS(Abstract, Blueprintable)
class IDAGOAP_API ARidaGOAPAIController : public AAIController
{
    GENERATED_BODY()

public:
    ARidaGOAPAIController();

    UFUNCTION(BlueprintCallable, Category = "GOAP")
    virtual void RequestGoal(TSubclassOf<UGOAPMotive> MotiveClass, const TArray<FGOAPFact>& SeedFacts);

    UFUNCTION(BlueprintCallable, Category = "GOAP|WorldState")
    void InjectWorldBool(const FString& Key, bool Value);

    UFUNCTION(BlueprintCallable, Category = "GOAP|WorldState")
    void InjectWorldInt(const FString& Key, int32 Value);

    UFUNCTION(BlueprintCallable, Category = "GOAP|WorldState")
    void InjectWorldFloat(const FString& Key, float Value);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
    float SenseRadius = 1500.f;

    UFUNCTION(BlueprintNativeEvent, Category = "GOAP")
    void SenseWorldState();
    virtual void SenseWorldState_Implementation() {}

    UFUNCTION(BlueprintCallable, Category = "GOAP")
    UGOAPAgentComponent* GetGOAPComponent() const;
};