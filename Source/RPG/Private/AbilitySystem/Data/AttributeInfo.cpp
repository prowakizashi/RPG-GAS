#include "AbilitySystem/Data/AttributeInfo.h"

#include "RPGLogChannels.h"


const FRPGAttributeInfo  FRPGAttributeInfo::EmptyInfo = FRPGAttributeInfo();

const FRPGAttributeInfo& UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag,
                                                                 bool bLogNotFound) const
{
	for (const FRPGAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogRpg, Error, TEXT("Info not found for Attribute Tag [%s] on Attribute Info [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}
	
	return FRPGAttributeInfo::EmptyInfo;
}
