#include "AbilitySystem/Data/AbilityInfo.h"

#include "RPGLogChannels.h"

const FRPGAbilityInfo  FRPGAbilityInfo::EmptyInfo = FRPGAbilityInfo();

bool UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, FRPGAbilityInfo& OutAbilityInfo, bool bLogNotFound) const
{
	for (const FRPGAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag.MatchesTagExact(AbilityTag))
		{
			OutAbilityInfo = Info;
			return true;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogRpg, Error, TEXT("Info not found for Ability Tag [%s] on Ability Info [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}

	return false;
}
