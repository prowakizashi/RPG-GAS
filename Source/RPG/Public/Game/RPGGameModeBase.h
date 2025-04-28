#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPGGameModeBase.generated.h"

class UAbilityInfo;
class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class RPG_API ARPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "RPG Properties|Character Class Info")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "RPG Properties|Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;
};
