#include "UI/WidgetControllers/WidgetControllerBase.h"

#include "RPGGameplayTags.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/RPGPlayerController.h"
#include "Player/RPGPlayerState.h"

void UWidgetControllerBase::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = Cast<ARPGPlayerController>(WCParams.PlayerController);
	PlayerState = Cast<ARPGPlayerState>(WCParams.PlayerState);
	AbilitySystemComponent = Cast<URPGAbilitySystemComponent>(WCParams.AbilitySystemComponent);
	AttributeSet = Cast<URPGAttributeSet>(WCParams.AttributeSet);
}

void UWidgetControllerBase::BroadcastAbilityInfo() const
{
	if (!AbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FRPGAbilityInfo Info;
		if (AbilityInfo->FindAbilityInfoForTag(URPGAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec), Info))
		{
			Info.InputTag = URPGAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
			Info.StatusTag = URPGAbilitySystemComponent::GetStatusFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	AbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UWidgetControllerBase::OnInitializeStartupAbilities() const
{
	if (!AbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FRPGAbilityInfo Info;
		if (AbilityInfo->FindAbilityInfoForTag(URPGAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec), Info))
		{
			Info.InputTag = URPGAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	AbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UWidgetControllerBase::OnAbilityEquipped(const FGameplayTag& EquippedAbilityTag,
	const FGameplayTag& UnEquippedAbilityTag, const FGameplayTag& InputTag, int32 AbilityLevel) const
{
	if (!AbilityInfo) return;

	const FRPGGameplayTags& Tags = FRPGGameplayTags::Get();
		
	FRPGAbilityInfo Info;
	if (UnEquippedAbilityTag.IsValid() && AbilityInfo->FindAbilityInfoForTag(UnEquippedAbilityTag, Info))
	{
		Info.StatusTag = Tags.Abilities_Status_Unlocked;
		Info.InputTag = FGameplayTag();
		AbilityInfoDelegate.Broadcast(Info);
	}
		
	if (AbilityInfo->FindAbilityInfoForTag(EquippedAbilityTag, Info))
	{
		Info.StatusTag = Tags.Abilities_Status_Equipped;
		Info.InputTag = InputTag;
		AbilityInfoDelegate.Broadcast(Info);
	}
}
