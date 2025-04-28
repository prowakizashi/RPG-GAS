#include "UI/WidgetControllers/OverlayWidgetController.h"

#include "RPGGameplayTags.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/RPGPlayerState.h"

void UOverlayWidgetController::BindCallbackDependencies() const
{
	PlayerState->XpChangedEvent.AddUObject(this, &UOverlayWidgetController::OnXpChanged);
	PlayerState->LevelChangedEvent.AddLambda([this](int32 NewLevel) { OnPlayerLevelChanged.Broadcast(NewLevel); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxManaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		});

	if (AbilitySystemComponent->bStartupAbilitiesGiven)
	{
		BroadcastAbilityInfo();
	}
	else
	{
		AbilitySystemComponent->AbilitiesGivenEvent.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
	}
	
	AbilitySystemComponent->AbilitiesEquippedEvent.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		
	AbilitySystemComponent->EffectAssetTagsEvent.AddLambda([this](const FGameplayTagContainer& AssetTags)
	{
		for (const FGameplayTag& Tag : AssetTags)
		{
			if (!Tag.MatchesTag(FGameplayTag::RequestGameplayTag("Message"))) continue;
			
			if (const FUIWidgetRow* Row = GetDatatableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag))
			{
				MessageWidgetRowDelegate.Broadcast(*Row);
			}
		}
	});
}

void UOverlayWidgetController::BroadcastInitialValues() const
{
	OnHealthChanged.Broadcast(AttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AttributeSet->GetMaxMana());
}

void UOverlayWidgetController::OnXpChanged(int32 NewXp) const
{
	const ULevelUpInfo* LevelUpInfo = PlayerState->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find Level Up Info. Please fill out RPG Player State Blueprint."))

	const int32 Level = LevelUpInfo->FindLevelIndexForXP(NewXp);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level < MaxLevel && Level >= 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = Level > 0 ? LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement : 0;
		const float DeltaRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const float DeltaDone = NewXp - PreviousLevelUpRequirement;
		OnXpPercentChanged.Broadcast(DeltaDone / DeltaRequirement);
	}
	else
	{
		OnXpPercentChanged.Broadcast(1.f);
	}
}
