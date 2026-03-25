#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GOAPAction.h"
#include "GOAPMotive.h"
#include "goap_agent.h"
#include "GOAPHeuristic.h"
#include "GOAPAgentComponent.generated.h"

UENUM(BlueprintType)
enum class EGOAPAgentStatus : uint8
{
    Idle            UMETA(DisplayName = "Idle"),
    Planning        UMETA(DisplayName = "Planning"),
    Executing       UMETA(DisplayName = "Executing"),
    PlanFailed      UMETA(DisplayName = "Plan Failed"),
    NoMotives       UMETA(DisplayName = "No Motives"),
    Interrupted     UMETA(DisplayName = "Interrupted")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAgentStatusChanged, EGOAPAgentStatus, NewStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionStarted, const FString&, ActionName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionFinished, const FString&, ActionName, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlanFound);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoalSatisfied, FString, MotiveName);

UCLASS(ClassGroup=(GOAP), meta=(BlueprintSpawnableComponent))
class IDAGOAP_API UGOAPAgentComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGOAPAgentComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP|Config")
    int32 GoalRecheckInterval = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP|Config")
    bool bReplanOnWorldChange = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP|Config")
    bool bSkipReplanSameGoal = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP|Config")
    int32 MaxConsecutiveFailures = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP|Config")
    bool bSynchronousPlanning = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP|Actions")
    TArray<TSubclassOf<UGOAPAction>> ActionClasses;

    UPROPERTY(BlueprintAssignable, Category = "GOAP|Events")
    FOnAgentStatusChanged OnStatusChanged;

    UPROPERTY(BlueprintAssignable, Category = "GOAP|Events")
    FOnActionStarted OnActionStarted;

    UPROPERTY(BlueprintAssignable, Category = "GOAP|Events")
    FOnActionFinished OnActionFinished;

    UPROPERTY(BlueprintAssignable, Category = "GOAP|Events")
    FOnPlanFound OnPlanFound;

    UPROPERTY(BlueprintAssignable, Category = "GOAP|Events")
    FOnGoalSatisfied OnGoalSatisfied;

    UPROPERTY(EditAnywhere, Category = "GOAP|Config")
    TSubclassOf<UGOAPHeuristic> HeuristicClass;

    UFUNCTION(BlueprintCallable, Category = "GOAP|WorldState")
    void SetWorldBool(const FString& Key, bool Value);

    UFUNCTION(BlueprintCallable, Category = "GOAP|WorldState")
    void SetWorldInt(const FString& Key, int32 Value);

    UFUNCTION(BlueprintCallable, Category = "GOAP|WorldState")
    void SetWorldFloat(const FString& Key, float Value);

	UFUNCTION(BlueprintCallable, Category = "GOAP|WorldState")
	void SetWorldPosition(const FString& Key, FVector Position);

    UFUNCTION(BlueprintCallable, Category = "GOAP|WorldState")
    void SetWorldString(const FString& Key, const FString& Value);

    UFUNCTION(BlueprintCallable, Category = "GOAP|WorldState")
    bool GetWorldBool(const FString& Key) const;

	UFUNCTION(BlueprintCallable, Category = "GOAP|WorldState")
	bool GetWorldPosition(const FString& Key, FVector& OutValue) const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP|Motives")
    TArray<TSubclassOf<UGOAPMotive>> MotiveClasses;

    UFUNCTION(BlueprintCallable, Category = "GOAP|Motives")
    void AddMotive(TSubclassOf<UGOAPMotive> MotiveClass);

    UFUNCTION(BlueprintCallable, Category = "GOAP|Motives")
    void ClearMotives();

    UFUNCTION(BlueprintCallable, Category = "GOAP|Motives")
    void SatisfyMotive(const FString& MotiveName);

    UFUNCTION(BlueprintCallable, Category = "GOAP|Motives")
    void SetMotivePriority(const FString& MotiveName, int32 NewPriority);

    UFUNCTION(BlueprintCallable, Category = "GOAP|Motives")
    FString GetActiveMotiveName() const;

    UFUNCTION(BlueprintCallable, Category = "GOAP")
    void Interrupt();

    UFUNCTION(BlueprintCallable, Category = "GOAP")
    void Resume();

    UFUNCTION(BlueprintCallable, Category = "GOAP")
    void ForceReplan();

    UFUNCTION(BlueprintCallable, Category = "GOAP")
    EGOAPAgentStatus GetAgentStatus() const;

    UFUNCTION(BlueprintCallable, Category = "GOAP")
    FString GetCurrentActionName() const;

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

private:
    TUniquePtr<rida_goap::goap_agent> Agent;

    UPROPERTY()
    TArray<UGOAPAction*> ActionInstances;

    UPROPERTY()
    TArray<UGOAPMotive*> MotiveInstances;

    UPROPERTY()
    UGOAPHeuristic* HeuristicInstance = nullptr;

    void InitialiseAgent();
    static EGOAPAgentStatus ConvertStatus(rida_goap::agent_status Status);
};
