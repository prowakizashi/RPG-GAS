#include "AbilitySystem/Abilities/RPGGameplayAbility.h"

#include "AbilitySystem/RPGAttributeSet.h"

FString URPGGameplayAbility::GetDescription(int32 Level) const
{
	return FString::Printf(TEXT("<Default>%hs, </><Level>%d</>"), "Default Ability Name - LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum", Level);
}

FString URPGGameplayAbility::GetNextLevelDescription(int32 Level) const
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n <Default>Causes much more damages.</>"), Level);
}

FString URPGGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell unlocked at level: </><Level>%d</>"), Level);
}

float URPGGameplayAbility::GetManaCost(float InLevel) const
{
	float ManaCost = 0.0f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (const FGameplayModifierInfo& Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == URPGAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel,ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float URPGGameplayAbility::GetCooldown(float InLevel) const
{
	float Cooldown = 0.0f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel,Cooldown);
	}
	return Cooldown;
}
