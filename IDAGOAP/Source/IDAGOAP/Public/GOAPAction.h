#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "goap_action.h"
#include "GOAPAction.generated.h"

UENUM(BlueprintType)
enum class EActionStatus : uint8
{
    Running     UMETA(DisplayName = "Running"),
    Succeeded   UMETA(DisplayName = "Succeeded"),
    Failed      UMETA(DisplayName = "Failed")
};

UCLASS(Abstract, Blueprintable, BlueprintType)
class IDAGOAP_API UGOAPAction : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GOAP")
    FString ActionName = TEXT("Unnamed Action");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GOAP")
    int32 Cost = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP|Preconditions")
	TMap<FString, bool> BoolPreconditions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP|Preconditions")
	TMap<FString, int32> IntPreconditions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP|Preconditions")
	TMap<FString, float> FloatPreconditions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP|Effects")
	TMap<FString, bool> BoolEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP|Effects")
	TMap<FString, int32> IntEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP|Effects")
	TMap<FString, float> FloatEffects;

	UPROPERTY(BlueprintReadOnly, Category = "GOAP")
	TWeakObjectPtr<AActor> OwningActor;

    UFUNCTION(BlueprintNativeEvent, Category = "GOAP")
    bool OnStart();
    virtual bool OnStart_Implementation() { return true; }

    UFUNCTION(BlueprintNativeEvent, Category = "GOAP")
    EActionStatus OnTick(float DeltaTime);
    virtual EActionStatus OnTick_Implementation(float DeltaTime)
    {
        return EActionStatus::Succeeded;
    }

    UFUNCTION(BlueprintNativeEvent, Category = "GOAP")
    void OnEnd(bool bSuccess);
    virtual void OnEnd_Implementation(bool bSuccess) {}

    UFUNCTION(BlueprintNativeEvent, Category = "GOAP")
    void OnInterrupt();
    virtual void OnInterrupt_Implementation() {}

    virtual rida_goap::goap_action::ptr BuildCoreAction();

protected:
    UFUNCTION(BlueprintCallable, Category = "GOAP")
    void AddPreconditionBool(const FString& Key, bool Value);

    UFUNCTION(BlueprintCallable, Category = "GOAP")
    void AddPreconditionInt(const FString& Key, int32 Value);

    UFUNCTION(BlueprintCallable, Category = "GOAP")
    void AddPreconditionFloat(const FString& Key, float Value);

    UFUNCTION(BlueprintCallable, Category = "GOAP")
    void AddEffectBool(const FString& Key, bool Value);

    UFUNCTION(BlueprintCallable, Category = "GOAP")
    void AddEffectInt(const FString& Key, int32 Value);

    UFUNCTION(BlueprintCallable, Category = "GOAP")
    void AddEffectFloat(const FString& Key, float Value);
};