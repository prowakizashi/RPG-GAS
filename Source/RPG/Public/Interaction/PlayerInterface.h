#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_API IPlayerInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 XP) const;

	UFUNCTION(BlueprintNativeEvent)
	void AddToXp(int32 XP);
	
	UFUNCTION(BlueprintNativeEvent)
	FRpgLevelUpInfo GetLevelUpInfo(int32 Level) const;
	
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp(int32 InPlayerLevelUps, int32 InAttributePoints, int32 InSpellPoints);
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetXp() const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints() const;
	
	UFUNCTION(BlueprintNativeEvent)
	void RemoveAttributePoints(int32 Points);
	
	UFUNCTION(BlueprintNativeEvent)
	void RemoveSpellPoints(int32 Points);
};
