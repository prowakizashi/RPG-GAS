#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ProjectileSpell.h"
#include "FireBolt.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UFireBolt : public UProjectileSpell
{
	GENERATED_BODY()

public:
	
	virtual FString GetDescription(int32 Level) const override;
	virtual FString GetNextLevelDescription(int32 Level) const override;
	
};
