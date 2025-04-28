#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FRPGAttributeInfo
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();
	
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
	
	static const FRPGAttributeInfo EmptyInfo;
};

/**
 * 
 */
UCLASS()
class RPG_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
public:

	const FRPGAttributeInfo& FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRPGAttributeInfo> AttributeInformation;
};
