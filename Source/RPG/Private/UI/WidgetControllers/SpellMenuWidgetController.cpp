#include "UI/WidgetControllers/SpellMenuWidgetController.h"

#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/RPGPlayerState.h"

void USpellMenuWidgetController::BindCallbackDependencies() const
{
	GetAbilitySystemComponent()->AbilitiesStatusChangedEvent.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
	{
		FRPGAbilityInfo Info;
		if (AbilityInfo && AbilityInfo->FindAbilityInfoForTag(AbilityTag, Info))
		{
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
	
	GetAbilitySystemComponent()->AbilitiesEquippedEvent.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);

	PlayerState->SpellPointsChangedEvent.AddLambda([this](int32 SpellPoints)
	{
		SpellPointsChangedEvent.Broadcast(SpellPoints);
	});
}

void USpellMenuWidgetController::BroadcastInitialValues() const
{
	BroadcastAbilityInfo();
	SpellPointsChangedEvent.Broadcast(PlayerState->GetSpellPoints());
}

void USpellMenuWidgetController::SpendSpellPoint(const FGameplayTag& AbilityTag)
{
	GetAbilitySystemComponent()->Server_SpendSpellPoint(AbilityTag);
}

void USpellMenuWidgetController::EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag)
{
	FRPGAbilityInfo Info;
	if (!AbilityInfo->FindAbilityInfoForTag(AbilityTag, Info)) return;
	GetAbilitySystemComponent()->Server_EquipAbility(AbilityTag, InputTag);
}

bool USpellMenuWidgetController::GetSpellDescriptions(const FGameplayTag& AbilityTag, FString& OutDescription,
                                                      FString& OutNextLevelDescription)
{
	return GetAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityInfo, AbilityTag, OutDescription, OutNextLevelDescription);
}
