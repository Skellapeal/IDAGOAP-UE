#include "GOAPAgentComponent.h"
#include "GOAPMotive.h"
#include <functional>

UGOAPAgentComponent::UGOAPAgentComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UGOAPAgentComponent::BeginPlay()
{
    Super::BeginPlay();
    InitialiseAgent();
}

void UGOAPAgentComponent::InitialiseAgent()
{
    rida_goap::agent_config Config;
    Config.goal_recheck_interval    = GoalRecheckInterval;
    Config.replan_on_world_change   = bReplanOnWorldChange;
    Config.skip_replan_same_goal    = bSkipReplanSameGoal;
    Config.max_consecutive_failures = MaxConsecutiveFailures;
    Config.synchronous_planning     = bSynchronousPlanning;

    Agent = MakeUnique<rida_goap::goap_agent>(Config);

    Agent->set_on_status_changed(rida_goap::goap_agent::status_changed_cb([this](rida_goap::agent_status, rida_goap::agent_status S)
    {
        OnStatusChanged.Broadcast(ConvertStatus(S));
    }));

    Agent->set_on_plan_found(rida_goap::goap_agent::plan_found_cb([this](const rida_goap::plan_result&)
    {
        OnPlanFound.Broadcast();
    }));

    Agent->set_on_action_started(rida_goap::goap_agent::action_started_cb([this](const rida_goap::goap_action& Action)
    {
		OnActionStarted.Broadcast(UTF8_TO_TCHAR(Action.get_name().c_str()));
    }));

    Agent->set_on_action_finished(rida_goap::goap_agent::action_finished_cb([this](const rida_goap::goap_action& Action, bool bSuccess)
    {
        OnActionFinished.Broadcast(UTF8_TO_TCHAR(Action.get_name().c_str()), bSuccess);
    }));

    Agent->set_on_goal_satisfied(rida_goap::goap_agent::goal_satisfied_cb([this](const rida_goap::motive& SatisfiedMotive)
    {
        OnGoalSatisfied.Broadcast(UTF8_TO_TCHAR(SatisfiedMotive.get_name().c_str()));
    }));

    for (const TSubclassOf<UGOAPAction>& ActionClass : ActionClasses)
    {
        if (!IsValid(ActionClass)) continue;
        UGOAPAction* Instance = ActionClass->GetDefaultObject<UGOAPAction>();
		Instance->OwningActor = GetOwner();
        Agent->add_action(Instance->BuildCoreAction());
    }

    for (const TSubclassOf<UGOAPMotive>& MotiveClass : MotiveClasses)
    {
        AddMotive(MotiveClass);
    }

    if (HeuristicClass)
    {
        HeuristicInstance = HeuristicClass->GetDefaultObject<UGOAPHeuristic>();
		Agent->set_heuristic(HeuristicInstance->BuildBridge());
    }
}

void UGOAPAgentComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (!Agent.IsValid()) return;

    Agent->tick(DeltaTime);
}

void UGOAPAgentComponent::SetWorldBool(const FString& Key, bool Value)
{
    if (Agent.IsValid()) Agent->get_world_state().set_bool(TCHAR_TO_UTF8(*Key), Value);
	Agent->mark_world_dirty();
}

bool UGOAPAgentComponent::GetWorldBool(const FString& Key) const
{
    if (!Agent.IsValid()) return false;
    return Agent->get_world_state().get_bool(TCHAR_TO_UTF8(*Key)).value_or(false);
}


void UGOAPAgentComponent::SetWorldInt(const FString& Key, int32 Value)
{
    if (Agent.IsValid()) Agent->get_world_state().set_int(TCHAR_TO_UTF8(*Key), Value);
	Agent->mark_world_dirty();
}

void UGOAPAgentComponent::SetWorldFloat(const FString& Key, float Value)
{
    if (Agent.IsValid()) Agent->get_world_state().set_float(TCHAR_TO_UTF8(*Key), Value);
	Agent->mark_world_dirty();
}

void UGOAPAgentComponent::SetWorldPosition(const FString& Key, FVector Value)
{
    if (!Agent.IsValid()) return;
    Agent->get_world_state().set_position(TCHAR_TO_UTF8(*Key), Value.X, Value.Y, Value.Z);
    Agent->mark_world_dirty();
}

bool UGOAPAgentComponent::GetWorldPosition(const FString& Key, FVector& OutValue) const
{
    if (!Agent.IsValid()) return false;

	std::optional<std::vector<float>> Result =
		Agent->get_world_state().get_position(TCHAR_TO_UTF8(*Key));

    if (!Result.has_value() || Result->size() < 3) return false;

    OutValue = FVector(Result->at(0), Result->at(1), Result->at(2));
    return true;
}

void UGOAPAgentComponent::SetWorldString(const FString& Key, const FString& Value)
{
    if (Agent.IsValid()) Agent->get_world_state().set_string(TCHAR_TO_UTF8(*Key), TCHAR_TO_UTF8(*Value));
	Agent->mark_world_dirty();
}

void UGOAPAgentComponent::Interrupt()   { if (Agent.IsValid()) Agent->interrupt(); }
void UGOAPAgentComponent::Resume()      { if (Agent.IsValid()) Agent->resume(); }
void UGOAPAgentComponent::ForceReplan() { if (Agent.IsValid()) Agent->force_replan(); }

EGOAPAgentStatus UGOAPAgentComponent::GetAgentStatus() const
{
    if (!Agent.IsValid()) return EGOAPAgentStatus::Idle;
    return ConvertStatus(Agent->get_status());
}

FString UGOAPAgentComponent::GetCurrentActionName() const
{
    if (!Agent.IsValid()) return FString();
    const auto Action = Agent->get_current_action();
    return Action ? UTF8_TO_TCHAR(Action->get_name().c_str()) : FString();
}

EGOAPAgentStatus UGOAPAgentComponent::ConvertStatus(rida_goap::agent_status Status)
{
    switch (Status)
    {
        case rida_goap::agent_status::Planning:      return EGOAPAgentStatus::Planning;
        case rida_goap::agent_status::Executing:     return EGOAPAgentStatus::Executing;
        case rida_goap::agent_status::PlanFailed:    return EGOAPAgentStatus::PlanFailed;
        case rida_goap::agent_status::Interrupted:   return EGOAPAgentStatus::Interrupted;
        default:                                     return EGOAPAgentStatus::Idle;
    }
}

void UGOAPAgentComponent::AddMotive(TSubclassOf<UGOAPMotive> MotiveClass)
{
    if (!IsValid(MotiveClass) || !Agent.IsValid()) return;

    UGOAPMotive* Instance = MotiveClass->GetDefaultObject<UGOAPMotive>();

    rida_goap::world_state GoalState;
    for (auto& [Key, Val] : Instance->GoalBools)
        GoalState.set_bool(TCHAR_TO_UTF8(*Key), Val);
    for (auto& [Key, Val] : Instance->GoalInts)
        GoalState.set_int(TCHAR_TO_UTF8(*Key), Val);
    for (auto& [Key, Val] : Instance->GoalFloats)
        GoalState.set_float(TCHAR_TO_UTF8(*Key), Val);

    auto CoreMotive = std::make_shared<rida_goap::motive>(
        TCHAR_TO_UTF8(*Instance->MotiveName),
        std::move(GoalState), static_cast<int>(Instance->Priority));

    CoreMotive->set_persistent(Instance->bPersistent);

    Agent->add_motive(CoreMotive);
}

void UGOAPAgentComponent::ClearMotives()
{
    if (Agent.IsValid()) Agent->clear_motives();
}

void UGOAPAgentComponent::SatisfyMotive(const FString& MotiveName)
{
    if (Agent.IsValid())
        Agent->satisfy_motive(TCHAR_TO_UTF8(*MotiveName));
}

void UGOAPAgentComponent::SetMotivePriority(const FString& MotiveName, int32 NewPriority)
{
    if (Agent.IsValid())
        Agent->set_motive_priority(TCHAR_TO_UTF8(*MotiveName), NewPriority);
}

FString UGOAPAgentComponent::GetActiveMotiveName() const
{
    if (!Agent.IsValid()) return FString();
    const auto Motive = Agent->get_active_motive();
    return Motive ? UTF8_TO_TCHAR(Motive->get_name().c_str()) : FString();
}