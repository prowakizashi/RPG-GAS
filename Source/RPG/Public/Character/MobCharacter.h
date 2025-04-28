#pragma once

#include "CoreMinimal.h"
#include "Character/RPGCharacterBase.h"
#include "Interaction/MobInterface.h"
#include "MobCharacter.generated.h"

class UBehaviorTree;
class AMobAIController;
struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewValue);

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class RPG_API AMobCharacter : public ARPGCharacterBase, public IMobInterface
{
	GENERATED_BODY()
public:
	AMobCharacter();

	virtual void PossessedBy(AController* NewController) override;
	
	/** Start MobInterface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual AActor* GetCombatTarget_Implementation() const override { return CombatTarget; }
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override { CombatTarget = InCombatTarget; }
	/** End MobInterface */
	
	/** Start CombatInterface */
	virtual void Die() override;
	virtual int32 GetCharacterLevel_Implementation() const override;
	/** End CombatInterface */

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor> CombatTarget = nullptr;

protected:
	
	virtual void BeginPlay() override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void InitAbilityActorInfo() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Class Default")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

private:

	UFUNCTION()
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AMobAIController> MobAIController;
};
