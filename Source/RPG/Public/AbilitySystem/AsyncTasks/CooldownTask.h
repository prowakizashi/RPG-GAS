#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CooldownTask.generated.h"

struct FGameplayEffectSpec;
class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangedSignature, float, TimeRemaining);

/**
 * 
 */
UCLASS(BlueprintType,  meta = (ExposedAsyncProxy = "AsyncTask"))
class RPG_API UCooldownTask : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FCooldownChangedSignature CooldownStarts;
	
	UPROPERTY(BlueprintAssignable)
	FCooldownChangedSignature CooldownEnds;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = true))
	static UCooldownTask* WaitForCooldownChange(UAbilitySystemComponent* InASC, const FGameplayTag& InCooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:

	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
	void OnActiveEffectAdded(UAbilitySystemComponent* InASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffect) const;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;
};
