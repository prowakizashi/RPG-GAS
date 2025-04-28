﻿#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FRpgLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelUpRequirement = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 AttributePointReward = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SpellPointReward = 1;
};

/**
 * 
 */
UCLASS()
class RPG_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRpgLevelUpInfo> LevelUpInformation;

	int32 FindLevelIndexForXP(int32 XP) const;
};
