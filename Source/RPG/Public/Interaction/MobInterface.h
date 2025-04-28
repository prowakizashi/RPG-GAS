#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MobInterface.generated.h"

UINTERFACE(MinimalAPI)
class UMobInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_API IMobInterface
{
	GENERATED_BODY()

public:
	
	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetCombatTarget() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCombatTarget(AActor* InCombatTarget);
};
