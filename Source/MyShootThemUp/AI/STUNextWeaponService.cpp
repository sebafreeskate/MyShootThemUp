// Nikita Kitov. All rights reserved


#include "STUNextWeaponService.h"

#include "AIController.h"
#include "STUAIWeaponComponent.h"

USTUNextWeaponService::USTUNextWeaponService()
{
	NodeName = "NextWeapon";
	Interval = 3.f;
	
}

void USTUNextWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	if(!Controller || !Controller->GetPawn())
	{
		return;
	}

	const float RandNormalizedFloat = 1.f / (FMath::Rand() % 10);

	if(RandNormalizedFloat <= Probability)
	{
		if(const auto WeaponComponent = Controller->GetPawn()->FindComponentByClass<USTUAIWeaponComponent>())
		{
			WeaponComponent->NextWeapon();	
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
