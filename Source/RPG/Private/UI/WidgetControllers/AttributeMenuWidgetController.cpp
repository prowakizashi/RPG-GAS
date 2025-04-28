#include "UI/WidgetControllers/AttributeMenuWidgetController.h"

#include "RPGGameplayTags.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "Player/RPGPlayerState.h"

void UAttributeMenuWidgetController::BindCallbackDependencies() const
{
	for (const auto& InfoPair : AttributeSet->GetTagsToAttributes())
	{
		const FGameplayTag& GameplayTag = InfoPair.Key;
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(InfoPair.Value())
			.AddLambda( [this, GameplayTag](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(GameplayTag, Data.NewValue);
			});
	}

	PlayerState->AttributePointsChangedEvent.AddLambda([this](int32 NewPoints)
	{
		NewAttributePointsChangedEvent.Broadcast(NewPoints);
	});
}

void UAttributeMenuWidgetController::BroadcastInitialValues() const
{
	for (const auto& InfoPair : AttributeSet->GetTagsToAttributes())
	{
		BroadcastAttributeInfo(InfoPair.Key, InfoPair.Value().GetNumericValue(AttributeSet));
	}

	NewAttributePointsChangedEvent.Broadcast(PlayerState->GetAttributePoints());
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& GameplayTag, float NewValue) const
{
	check(AttributeInfo);
	if (const FOnAttributeChangedMultiDelegate* MultiDelegate = AttributeChangedDelegateMap.Find(GameplayTag))
	{
		FRPGAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(GameplayTag);
		Info.AttributeValue = NewValue;
		MultiDelegate->Broadcast(Info);
	}
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag) const
{
	AbilitySystemComponent->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::RegisterToAttributeChange(FGameplayTag GameplayTag,
                                                               const FOnAttributeChangedDelegate& OnAttributeChangedDelegate)
{
	if (FOnAttributeChangedMultiDelegate* MultiDelegate = AttributeChangedDelegateMap.Find(GameplayTag))
	{
		MultiDelegate->Add(OnAttributeChangedDelegate);
	}
	else
	{
		FOnAttributeChangedMultiDelegate NewMultiDelegate;
		NewMultiDelegate.Add(OnAttributeChangedDelegate);
		AttributeChangedDelegateMap.Add(GameplayTag, NewMultiDelegate);
	}
}
void UAttributeMenuWidgetController::UnRegisterToAttributeChange(FGameplayTag GameplayTag,
																const FOnAttributeChangedDelegate& OnAttributeChangedDelegate)
{
	if (FOnAttributeChangedMultiDelegate* MultiDelegate = AttributeChangedDelegateMap.Find(GameplayTag))
	{
		MultiDelegate->Remove(OnAttributeChangedDelegate);
	}
}

void UAttributeMenuWidgetController::UnRegisterAllToAttributeChange(const UObject* WorldContextObject)
{
	for (TTuple<FGameplayTag, FOnAttributeChangedMultiDelegate>& Pair : AttributeChangedDelegateMap)
	{
		Pair.Value.RemoveAll(WorldContextObject);
	}
}
