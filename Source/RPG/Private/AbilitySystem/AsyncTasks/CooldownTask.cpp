#include "AbilitySystem/AsyncTasks/CooldownTask.h"

#include "AbilitySystemComponent.h"

UCooldownTask* UCooldownTask::WaitForCooldownChange(UAbilitySystemComponent* InASC, const FGameplayTag& InCooldownTag)
{
	UCooldownTask* Task = NewObject<UCooldownTask>();
	Task->ASC = InASC;
	Task->CooldownTag = InCooldownTag;

	if (!IsValid(InASC) || !InCooldownTag.IsValid())
	{
		Task->EndTask();
		return nullptr;
	}
	
	// To know when a cooldown has ended (Cooldown Tag has been removed)
	InASC->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(Task, &UCooldownTask::CooldownTagChanged);
	// To know when a cooldown effect has been applied
	InASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(Task, &UCooldownTask::OnActiveEffectAdded);
	return Task;
}

void UCooldownTask::EndTask()
{
	if (!IsValid(ASC)) return;
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UCooldownTask::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CooldownEnds.Broadcast(0.f);
	}
}

void UCooldownTask::OnActiveEffectAdded(UAbilitySystemComponent* InASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffect) const
{
	FGameplayTagContainer AssetTags;
	EffectSpec.GetAllAssetTags(AssetTags);
	
	FGameplayTagContainer GrantedTags;
	EffectSpec.GetAllGrantedTags(GrantedTags);
	
	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> RemainingTimes = ASC->GetActiveEffectsTimeRemaining(Query);
		
		// There should be only 1 float in the array
		if (RemainingTimes.Num() > 0)
		{
			CooldownStarts.Broadcast(RemainingTimes[0]);
		}
	}
}
