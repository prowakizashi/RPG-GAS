#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/WidgetControllerBase.h"
#include "SpellMenuWidgetController.generated.h"

struct FGameplayTag;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class RPG_API USpellMenuWidgetController : public UWidgetControllerBase
{
	GENERATED_BODY()

public:
	
	virtual void BindCallbackDependencies() const override;
	virtual void BroadcastInitialValues() const override;

	UFUNCTION(BlueprintCallable)
	void SpendSpellPoint(const FGameplayTag& AbilityTag);
	
	UFUNCTION(BlueprintCallable)
	void EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag);
	
	UFUNCTION(BlueprintCallable)
	bool GetSpellDescriptions(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);
	
	UPROPERTY(BlueprintAssignable)
	FPlayerStatChangedSignature SpellPointsChangedEvent;
};
