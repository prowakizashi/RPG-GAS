#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "MobAIController.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class RPG_API AMobAIController : public AAIController
{
	GENERATED_BODY()

public:
	
	AMobAIController();

protected:
	
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
