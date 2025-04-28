#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "RPGCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class USkeletalMeshComponent;
class UAttributeSet;
class UNiagaraSystem;

UCLASS(Abstract)
class RPG_API ARPGCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ARPGCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
	/** Start CombatInterface */
	virtual void Die() override;
	virtual int32 GetCharacterLevel_Implementation() const override { return 0; }
	virtual ECharacterClass GetCharacterClass_Implementation() const override { return CharacterClass; }
	virtual int32 GetSummonedCount_Implementation() const override { return SummonedCount; }
	virtual void IncrementSummonedCount_Implementation(int32 Amount = 1) override { SummonedCount += Amount; }
	virtual void DecrementSummonedCount_Implementation(int32 Amount = 1) override { SummonedCount -= Amount; }
	virtual bool IsDead_Implementation() const override { return bDead; }
	virtual AActor* GetAvatar_Implementation() override { return this;}
	virtual FVector GetAttackSocketLocation_Implementation(const FGameplayTag& SocketTag) const override;
	virtual UAnimMontage* GetHitReactAnimMontage_Implementation() override { return HitReactMontage;}
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() const override { return AttackMontages;}
	virtual UNiagaraSystem* GetBloodEffect_Implementation() const override { return BloodEffect; }
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& GameplayTag) const override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() const override;
	/** End CombatInterface */

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;

protected:
	
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() {}
	virtual void InitializeDefaultAttributes() const;

	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_OnDeath();
	
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const;
	void AddCharacterAbilities() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,	Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	FName WeaponOriginSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	FName LeftHandOriginSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	FName RightHandOriginSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	FName TailOriginSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribues")
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribues")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribues")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribues")
	TSubclassOf<UGameplayEffect> DefaultVitalsAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	UNiagaraSystem* BloodEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	USoundBase* DeathSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Class Default")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	/* Dissolve Effect */

	void Dissolve();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	/* Summoned Creature */
	
	int32 SummonedCount = 0;
	
private:
	
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	bool bDead = false;
};
