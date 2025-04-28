#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FRPGAbilityInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequirement = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;
	
	static const FRPGAbilityInfo EmptyInfo;
};

/**
 * 
 */
UCLASS()
class RPG_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	bool FindAbilityInfoForTag(const FGameplayTag& AbilityTag, FRPGAbilityInfo& OutAbilityInfo, bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Information")
	TArray<FRPGAbilityInfo> AbilityInformation;
};
