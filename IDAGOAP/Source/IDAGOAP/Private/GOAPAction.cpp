#include "GOAPAction.h"
#include "goap_action.h"

class FUGOAPActionBridge final : public rida_goap::goap_action
{
public:
    explicit FUGOAPActionBridge(UGOAPAction* InOwner)
        : goap_action(TCHAR_TO_UTF8(*InOwner->ActionName), InOwner->Cost)
        , Owner(InOwner)
    {}

    bool on_start() override
    {
        return Owner.IsValid() ? Owner->OnStart() : false;
    }

    rida_goap::action_status on_tick(float DeltaTime) override
    {
        if (!Owner.IsValid()) return rida_goap::action_status::Failed;
        switch (Owner->OnTick(DeltaTime))
        {
            case EActionStatus::Running:   return rida_goap::action_status::Running;
            case EActionStatus::Succeeded: return rida_goap::action_status::Succeeded;
            default:                       return rida_goap::action_status::Failed;
        }
    }

    void on_end(bool bSuccess) override
    {
        if (Owner.IsValid()) Owner->OnEnd(bSuccess);
    }

    void on_interrupt() override
    {
        if (Owner.IsValid()) Owner->OnInterrupt();
    }

private:
    TWeakObjectPtr<UGOAPAction> Owner;
};

void UGOAPAction::AddPreconditionBool(const FString& Key, bool Value)  { BoolPreconditions.Add(Key, Value); }
void UGOAPAction::AddPreconditionInt(const FString& Key, int32 Value)  { IntPreconditions.Add(Key, Value); }
void UGOAPAction::AddPreconditionFloat(const FString& Key, float Value){ FloatPreconditions.Add(Key, Value); }

void UGOAPAction::AddEffectBool(const FString& Key, bool Value)  { BoolEffects.Add(Key, Value); }
void UGOAPAction::AddEffectInt(const FString& Key, int32 Value)  { IntEffects.Add(Key, Value); }
void UGOAPAction::AddEffectFloat(const FString& Key, float Value){ FloatEffects.Add(Key, Value); }

rida_goap::goap_action::ptr UGOAPAction::BuildCoreAction()
{
    auto bridge = std::make_shared<FUGOAPActionBridge>(this);

    for (auto& [Key, Val] : BoolPreconditions)
        bridge->add_precondition(TCHAR_TO_UTF8(*Key), Val);
    for (auto& [Key, Val] : IntPreconditions)
        bridge->add_precondition(TCHAR_TO_UTF8(*Key), Val);
    for (auto& [Key, Val] : FloatPreconditions)
        bridge->add_precondition(TCHAR_TO_UTF8(*Key), Val);

    for (auto& [Key, Val] : BoolEffects)
        bridge->add_effect(TCHAR_TO_UTF8(*Key), Val);
    for (auto& [Key, Val] : IntEffects)
        bridge->add_effect(TCHAR_TO_UTF8(*Key), Val);
    for (auto& [Key, Val] : FloatEffects)
        bridge->add_effect(TCHAR_TO_UTF8(*Key), Val);

    return bridge;
}
