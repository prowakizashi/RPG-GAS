#include "Player/RPGPlayerState.h"

#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "Net/UnrealNetwork.h"

ARPGPlayerState::ARPGPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<URPGAttributeSet>("AttributeSet");
	
	SetNetUpdateFrequency(100.f);
}

void ARPGPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPGPlayerState, Level);
	DOREPLIFETIME(ARPGPlayerState, Xp);
	DOREPLIFETIME(ARPGPlayerState, AttributePoints);
	DOREPLIFETIME(ARPGPlayerState, SpellPoints);
}

UAbilitySystemComponent* ARPGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARPGPlayerState::SetXp(int32 InXp)
{
	if (HasAuthority())
	{
		Xp = InXp;
		XpChangedEvent.Broadcast(Xp);
	}
}

void ARPGPlayerState::SetLevel(int32 InLevel)
{
	if (HasAuthority())
	{
		Level = InLevel;
		LevelChangedEvent.Broadcast(InLevel);
	}
}

void ARPGPlayerState::SetAttributePoints(int32 InAttributePoints)
{
	if (HasAuthority())
	{
		AttributePoints = InAttributePoints;
		AttributePointsChangedEvent.Broadcast(AttributePoints);
	}
}

void ARPGPlayerState::SetSpellPoints(int32 InSpellPoints)
{
	if (HasAuthority())
	{
		SpellPoints = InSpellPoints;
		SpellPointsChangedEvent.Broadcast(SpellPoints);
	}
}

void ARPGPlayerState::AddXp(int32 InXp)
{
	if (HasAuthority())
	{
		Xp += InXp;
		XpChangedEvent.Broadcast(Xp);
	}
}

void ARPGPlayerState::AddToLevel(int32 InLevel)
{
	if (HasAuthority())
	{
		Level += InLevel;
		LevelChangedEvent.Broadcast(Level);
	}
}

void ARPGPlayerState::AddAttributePoints(int32 InAttributePoints)
{
	if (HasAuthority())
	{
		AttributePoints += InAttributePoints;
		AttributePointsChangedEvent.Broadcast(AttributePoints);
	}
}

void ARPGPlayerState::AddSpellPoints(int32 InSpellPoints)
{
	if (HasAuthority())
	{
		SpellPoints += InSpellPoints;
		SpellPointsChangedEvent.Broadcast(SpellPoints);
	}
}

void ARPGPlayerState::OnRep_Xp(int32 OldXp)
{
	XpChangedEvent.Broadcast(Xp);
}

void ARPGPlayerState::OnRep_Level(int32 OldLevel)
{
	LevelChangedEvent.Broadcast(Level);
}

void ARPGPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	AttributePointsChangedEvent.Broadcast(AttributePoints);
}

void ARPGPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	SpellPointsChangedEvent.Broadcast(SpellPoints);
}
