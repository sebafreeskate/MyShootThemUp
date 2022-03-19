// Nikita Kitov. All rights reserved


#include "STUPickupHeal.h"

#include "STUHealthComponent.h"
#include "STUUtils.h"

bool ASTUPickupHeal::GivePickupTo(APawn* Pawn)
{
	auto HealthComponent = GetPawnComponent<USTUHealthComponent>(Pawn);
	if(!HealthComponent)
	{
		return false;
	}

	return HealthComponent->TryHeal(HealAmount);
	
	return true;
}
