#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelIndexForXP(int32 XP) const
{
	int32 Level = 0;
	for (; Level < LevelUpInformation.Num() && XP >= LevelUpInformation[Level].LevelUpRequirement ; ++Level)
	{
		XP -= LevelUpInformation[Level].LevelUpRequirement;
	}
	return Level;
}
