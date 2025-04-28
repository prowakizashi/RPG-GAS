#pragma once

#include "CoreMinimal.h"
#include "Character/RPGCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "RPGPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class RPG_API ARPGPlayerCharacter : public ARPGCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
public:
	ARPGPlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;
	
	// At this time we know that Player State is valid on local player, so should also be the Player Controller
	virtual void OnRep_PlayerState() override;
	
	/** Start CombatInterface */
	virtual int32 GetCharacterLevel_Implementation() const override;
	/** End CombatInterface */
	
	/** Start PlayerInterface */
	virtual int32 FindLevelForXP_Implementation(int32 XP) const override;
	virtual int32 GetXp_Implementation() const override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual FRpgLevelUpInfo GetLevelUpInfo_Implementation(int32 Level) const override;
	virtual void RemoveAttributePoints_Implementation(int32 Points) override;
	virtual void RemoveSpellPoints_Implementation(int32 Points) override;
	virtual void AddToXp_Implementation(int32 XP) override;
	virtual void LevelUp_Implementation(int32 InPlayerLevelUps, int32 InAttributePoints, int32 InSpellPoints) override;
	/** End PlayerInterface */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
	
protected:
	
	virtual void InitAbilityActorInfo() override;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_LevelUpParticles();
};
