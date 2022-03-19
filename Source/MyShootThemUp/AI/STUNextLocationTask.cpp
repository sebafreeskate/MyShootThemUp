// Nikita Kitov. All rights reserved


#include "STUNextLocationTask.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

USTUNextLocationTask::USTUNextLocationTask()
{
	NodeName = "Next Location";
}

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto OwnerController = OwnerComp.GetAIOwner();
	const auto OwnerPawn = OwnerController->GetPawn();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	if(!OwnerController || !OwnerPawn || !Blackboard)
	{
		return EBTNodeResult::Failed;
	}

	const auto NavSystem = UNavigationSystemV1::GetCurrent(OwnerPawn);
	auto Location = OwnerPawn->GetActorLocation();
	if(!SelfCenter)
	{
		const auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
		if(!CenterActor)
		{
			return EBTNodeResult::Failed;
		}
		Location = CenterActor->GetActorLocation();
	}

	if(!NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	FNavLocation FoundLocation;
	const bool bFound =NavSystem->GetRandomReachablePointInRadius(OwnerPawn->GetActorLocation(), Radius, FoundLocation);	

	if(!bFound)
	{
		return EBTNodeResult::Failed;
	}

	Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, FoundLocation.Location);

	return EBTNodeResult::Succeeded;
	
}
