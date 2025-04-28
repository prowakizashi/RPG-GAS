#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RPGGameplayAbility.h"
#include "SummonSpell.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API USummonSpell : public URPGGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Summoning")
	TArray<FVector> GetSpawnLocations();
	
	UFUNCTION(BlueprintCallable, Category = "Summoning")
	TSubclassOf<APawn> GetRandomSummonedClass() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Summoning")
	int32 NumSummoned = 5;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Summoning")
	TArray<TSubclassOf<APawn>> SummonedClasses;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Summoning")
	float MinSpawnDistance = 150.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Summoning")
	float MaxSpawnDistance = 400.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Summoning")
	float SpawnSpread = 90.f;
};
