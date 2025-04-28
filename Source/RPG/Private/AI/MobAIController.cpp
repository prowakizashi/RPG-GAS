#include "AI/MobAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AMobAIController::AMobAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(Blackboard);
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(BehaviorTreeComponent);
}