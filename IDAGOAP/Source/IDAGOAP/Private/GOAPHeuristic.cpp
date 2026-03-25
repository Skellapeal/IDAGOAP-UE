#include <memory>
#include "GOAPHeuristic.h"
#include "heuristic.h"
#include "world_state.h"

class FGOAPHeuristicBridge : public rida_goap::heuristic
{
public:
    explicit FGOAPHeuristicBridge(UGOAPHeuristic* InOwner) : Owner(InOwner) {}

    int estimate(const rida_goap::world_state& Current, const rida_goap::world_state& Goal) const override
    {
        if (!Owner.IsValid()) return 0;

        TMap<FString, bool> CurrentBools, GoalBools;

        for (auto& [Key, Val] : Current.get_states())
            if (const bool* b = std::get_if<bool>(&Val))
                CurrentBools.Add(UTF8_TO_TCHAR(Key.c_str()), *b);

        for (auto& [Key, Val] : Goal.get_states())
            if (const bool* b = std::get_if<bool>(&Val))
                GoalBools.Add(UTF8_TO_TCHAR(Key.c_str()), *b);

        return Owner->Estimate(CurrentBools, GoalBools);
    }

private:
    TWeakObjectPtr<UGOAPHeuristic> Owner;
};

std::shared_ptr<rida_goap::heuristic> UGOAPHeuristic::BuildBridge()
{
    return std::make_shared<FGOAPHeuristicBridge>(this);
}
