#pragma once

#include "CoreMinimal.h"
#include "RPGAbilityTypes.h"
#include "RPGGameplayAbility.h"
#include "DamageAbility.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UDamageAbility : public URPGGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

	UFUNCTION(BlueprintCallable)
	float GetDamageAtLevel() const;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat Damage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffChance = 20.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffDamage = 5.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffFrequency = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffDuration = 5.f;
};
