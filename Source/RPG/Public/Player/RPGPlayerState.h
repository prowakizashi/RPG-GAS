#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "RPGPlayerState.generated.h"

class ULevelUpInfo;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FStatChangedSignature, int32 /* StatValue */);

/**
 * 
 */
UCLASS()
class RPG_API ARPGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	
	ARPGPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	FORCEINLINE int32 GetCharacterLevel() const { return Level; }
	FORCEINLINE int32 GetCharacterXp() const { return Xp; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }

	void SetXp(int32 InXp);
	void SetLevel(int32 InLevel);
	void SetAttributePoints(int32 InAttributePoints);
	void SetSpellPoints(int32 InSpellPoints);
	
	void AddXp(int32 InXp);
	void AddToLevel(int32 InLevel);
	void AddAttributePoints(int32 InAttributePoints);
	void AddSpellPoints(int32 InSpellPoints);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;
	
	FStatChangedSignature XpChangedEvent;
	FStatChangedSignature LevelChangedEvent;
	FStatChangedSignature AttributePointsChangedEvent;
	FStatChangedSignature SpellPointsChangedEvent;

protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
private:
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 Xp = 0;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SpellPoints)
	int32 SpellPoints = 0;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_Xp(int32 OldXp);
	
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);
	
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
};
