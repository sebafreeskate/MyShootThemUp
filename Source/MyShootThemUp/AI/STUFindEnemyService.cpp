// Nikita Kitov. All rights reserved


#include "STUFindEnemyService.h"

#include "AIController.h"
#include "STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USTUFindEnemyService::USTUFindEnemyService()
{
	NodeName = TEXT("Find Enemy");
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if(Blackboard)
	{
		const auto Controller = OwnerComp.GetAIOwner();
		if(Controller)
		{
			const auto PerceptionComponent = Controller->FindComponentByClass<USTUAIPerceptionComponent>();
			if(PerceptionComponent)
			{
				Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy()); 
			}
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
