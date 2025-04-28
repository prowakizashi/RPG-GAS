#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouseTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataDelegateSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 * 
 */
UCLASS()
class RPG_API UTargetDataUnderMouseTask : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", DisplayName = "TargetDataUnderMouseTask",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = true))
	static UTargetDataUnderMouseTask* CreateTargetDataUnderMouseTask(UGameplayAbility* OwningAbility);


	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataDelegateSignature OnGetTargetData;

protected: 
	virtual void Activate() override;

private:
	void SendMouseCursorData() const;
	UFUNCTION()
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag) const;
};
