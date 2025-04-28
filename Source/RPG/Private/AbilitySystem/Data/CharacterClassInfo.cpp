#include "AbilitySystem/Data/CharacterClassInfo.h"

const FCharacterClassDefaultInfo& UCharacterClassInfo::GetCharacterClassDefaultInfo(
	ECharacterClass CharacterClass) const
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}
