

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DamageAbility.h"
#include "BeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBeamSpell : public UDamageAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);
	
	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();
	
	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);
	
	UFUNCTION(BlueprintCallable)
	void StoreAdditionnalTargets(TArray<AActor*>& OutAdditionalTargets);
	
protected:

	UPROPERTY(BlueprintReadWrite, Category="Beam")
	FVector MouseHitLocation;
	
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<AActor> MouseHitActor;
	
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;
	
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<ACharacter> OwnerCharacter;
	
	UPROPERTY(EditDefaultsOnly, Category="Beam")
	int32 MaxNumShockTargets = 5;
};
