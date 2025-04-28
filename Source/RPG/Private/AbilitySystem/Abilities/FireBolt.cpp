#include "AbilitySystem/Abilities/FireBolt.h"

#include "RPGGameplayTags.h"

// TODO: I don't like the fact this is hard coded... It should be set in a FText (for localization) with replacement symbols (to insert values here).
FString UFireBolt::GetDescription(int32 Level) const
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(GetAbilityLevel()));
	const float Cooldown = GetCooldown(GetAbilityLevel());
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT: </>\n"
			"\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"\n"
			"<Default>Launches a bolt of fire, "
			"exploding on impact and dealing </><Damage>%d</><Default> fire damage with a chance to burn.</>"),
			Level, ManaCost, Cooldown,ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT: </>\n"
			"\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"\n"
			"<Default>Launches %d bolts of fire, "
			"exploding on impact and dealing </><Damage>%d</><Default> fire damage with a chance to burn.</>"),
			Level, ManaCost, Cooldown,FMath::Min(NumProjectiles, Level), ScaledDamage);
	}
}

FString UFireBolt::GetNextLevelDescription(int32 Level) const
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(GetAbilityLevel()));
	const float Cooldown = GetCooldown(GetAbilityLevel());
	
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL: </>\n"
		"\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n"
		"\n"
		"<Default>Launches %d bolts of fire, "
		"exploding on impact and dealing </><Damage>%d</><Default> fire damage with a chance to burn.</>"),
			Level, ManaCost, Cooldown,FMath::Min(NumProjectiles, Level), ScaledDamage);
}
