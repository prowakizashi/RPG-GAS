#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "UI/WidgetControllers/WidgetControllerBase.h"
#include "AttributeMenuWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedMultiDelegate, const FRPGAttributeInfo&, AttributeInfo);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAttributeChangedDelegate, const FRPGAttributeInfo&, AttributeInfo);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class RPG_API UAttributeMenuWidgetController : public UWidgetControllerBase
{
	GENERATED_BODY()
	
public:

	virtual void BindCallbackDependencies() const override;
	virtual void BroadcastInitialValues() const override;

	void BroadcastAttributeInfo(const FGameplayTag& GameplayTag, float NewValue) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "GAS|Attributes")
	void UpgradeAttribute(const FGameplayTag& AttributeTag) const;
	
	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes")
	void RegisterToAttributeChange(FGameplayTag GameplayTag, const FOnAttributeChangedDelegate& OnAttributeChangedDelegate);
	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes")
	void UnRegisterToAttributeChange(FGameplayTag GameplayTag, const FOnAttributeChangedDelegate& OnAttributeChangedDelegate);
	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes", meta=(WorldContext = "WorldContextObject"))
	void UnRegisterAllToAttributeChange(const UObject* WorldContextObject);

	UPROPERTY(EditDefaultsOnly, Category = "GAS|Attributes")
	TObjectPtr<UAttributeInfo> AttributeInfo;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FPlayerStatChangedSignature NewAttributePointsChangedEvent;
	
private:

	TMap<FGameplayTag, FOnAttributeChangedMultiDelegate> AttributeChangedDelegateMap;
	
};
