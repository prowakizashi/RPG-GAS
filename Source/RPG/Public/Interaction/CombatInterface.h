#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"


class UNiagaraSystem;
class UAnimMontage;

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;
};

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_API ICombatInterface
{
	GENERATED_BODY()

public:

	virtual void Die() = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetCharacterLevel() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClass GetCharacterClass() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetSummonedCount() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IncrementSummonedCount(int32 Amount = 1);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DecrementSummonedCount(int32 Amount = 1);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetAttackSocketLocation(const FGameplayTag& SocketTag) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactAnimMontage();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintPure = false)
	void UpdateFacingTarget(const FVector& TargetLocation) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetBloodEffect() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& GameplayTag) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetInShockLoop(bool InLoop);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetWeapon() const;
};
