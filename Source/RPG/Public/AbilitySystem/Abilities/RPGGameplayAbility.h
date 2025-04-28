#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RPGGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:

	virtual FString GetDescription(int32 Level) const;
	virtual FString GetNextLevelDescription(int32 Level) const;
	static FString GetLockedDescription(int32 Level);
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

protected:

	float GetManaCost(float InLevel) const;
	float GetCooldown(float InLevel) const;
};
