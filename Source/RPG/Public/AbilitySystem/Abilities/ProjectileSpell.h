#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DamageAbility.h"
#include "ProjectileSpell.generated.h"

class AProjectileBase;
/**
 * 
 */
UCLASS()
class RPG_API UProjectileSpell : public UDamageAbility
{
	GENERATED_BODY()

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AProjectileBase> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumProjectiles = 5;
};
