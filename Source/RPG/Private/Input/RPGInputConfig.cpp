#include "Input/RPGInputConfig.h"

#include "InputAction.h"

const UInputAction* URPGInputConfig::FindAbilityInputActionForTag(const FGameplayTag InputTag, bool bLogNotFound) const
{
	for (const FRpgInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Ability Input Action not found for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}
	
	return nullptr;
}
