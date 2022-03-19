// Nikita Kitov. All rights reserved


#include "STUPickupAmmo.h"


#include "STUHealthComponent.h"
#include "STUUtils.h"
#include "STUWeaponComponent.h"

bool ASTUPickupAmmo::GivePickupTo(APawn* Pawn)
{
	if(AmmoClips == 0)
	{
		UE_LOG(LogPickupAmmo, Warning, TEXT("Ammo clips = %d"), AmmoClips);
		return false;
	}

	const auto HealthComponent = GetPawnComponent<USTUHealthComponent>(Pawn);
	if(!HealthComponent || HealthComponent->IsDead())
	{
		UE_LOG(LogPickupAmmo, Warning, TEXT("Pawn %s is dead "), *GetNameSafe(Pawn));
		return false;
	}

	const auto WeaponComponent = GetPawnComponent<USTUWeaponComponent>(Pawn);
	if(!WeaponComponent)
	{
		UE_LOG(LogPickupAmmo, Warning, TEXT("Pawn %s has ho WeaponComponent "), *GetNameSafe(Pawn));
		return false;
	}

	return WeaponComponent->TryToAddAmmo(WeaponClass, AmmoClips);
}
