// Nikita Kitov. All rights reserved


#include "STUFireService.h"

#include "AIController.h"
#include "STUAICharacter.h"
#include "STUWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USTUFireService::USTUFireService()
{
	NodeName = "Fire";
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	const bool HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

	if(Controller && Controller->GetPawn())
	{
		if(const auto WeaponComponent = Controller->GetPawn()->FindComponentByClass<USTUWeaponComponent>())
		{
			HasAim? WeaponComponent->StartFire() : WeaponComponent->StopFire();
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
