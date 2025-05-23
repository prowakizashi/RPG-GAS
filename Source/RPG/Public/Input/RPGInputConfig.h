#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "RPGInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FRpgInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class RPG_API URPGInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:

	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag InputTag, bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRpgInputAction> AbilityInputActions;
};
