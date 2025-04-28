#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WidgetControllerBase.generated.h"

class UAbilityInfo;
class URPGAttributeSet;
class ARPGPlayerState;
class ARPGPlayerController;
class URPGAbilitySystemComponent;
class UAttributeSet;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerStatChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FRPGAbilityInfo&, AbilityInfo);

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

/**
 * 
 */
UCLASS()
class RPG_API UWidgetControllerBase : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	virtual void BindCallbackDependencies() const {}
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() const {}
	void BroadcastAbilityInfo() const;

	ARPGPlayerController* GetPlayerController() const { return PlayerController; }
	ARPGPlayerState* GetPlayerState() const { return PlayerState;}
	URPGAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent;}
	URPGAttributeSet* GetAttributeSet() const { return AttributeSet;}
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FAbilityInfoSignature AbilityInfoDelegate;
	
protected:

	UFUNCTION()
	void OnInitializeStartupAbilities() const;

	UFUNCTION()
	void OnAbilityEquipped(const FGameplayTag& EquippedAbilityTag, const FGameplayTag& UnEquippedAbilityTag, const FGameplayTag& InputTag, int32 AbilityLevel) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;
	
	UPROPERTY(BlueprintReadOnly, Category = WidgetController)
	TObjectPtr<ARPGPlayerController> PlayerController;
	
	UPROPERTY(BlueprintReadOnly, Category = WidgetController)
	TObjectPtr<ARPGPlayerState> PlayerState;
	
	UPROPERTY(BlueprintReadOnly, Category = WidgetController)
	TObjectPtr<URPGAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = WidgetController)
	TObjectPtr<URPGAttributeSet> AttributeSet;
};
