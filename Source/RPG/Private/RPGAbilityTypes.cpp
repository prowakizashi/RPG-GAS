#include "RPGAbilityTypes.h"

bool FRPGGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	uint16 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bIsBlockedHit)
		{
			RepBits |= 1 << 0;
		}
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 1;
		}
		if (bIsSuccessfulDebuff)
		{
			RepBits |= 1 << 2;
		}
		if (DebuffDamage > 0.f)
		{
			RepBits |= 1 << 3;
		}
		if (DebuffDuration > 0.f)
		{
			RepBits |= 1 << 4;
		}
		if (DebuffFrequency > 0.f)
		{
			RepBits |= 1 << 5;
		}
		if (DamageType.IsValid())
		{
			RepBits |= 1 << 6;
		}
	}

	Ar.SerializeBits(&RepBits, 7);

	if (RepBits & (1 << 0))
	{
		Ar << bIsBlockedHit;
	}
	if (RepBits & (1 << 1))
	{
		Ar << bIsCriticalHit;
	}
	if (RepBits & (1 << 2))
	{
		Ar << bIsSuccessfulDebuff;
	}
	if (RepBits & (1 << 3))
	{
		Ar << DebuffDamage;
	}
	if (RepBits & (1 << 4))
	{
		Ar << DebuffDuration;
	}
	if (RepBits & (1 << 5))
	{
		Ar << DebuffFrequency;
	}
	if (RepBits & (1 << 6))
	{
		if (Ar.IsLoading())
		{
			if (!DamageType.IsValid())
			{
				DamageType = MakeShared<FGameplayTag>();
			}
		}
		DamageType->NetSerialize(Ar, Map, bOutSuccess);
	}
	
	return FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);
}
