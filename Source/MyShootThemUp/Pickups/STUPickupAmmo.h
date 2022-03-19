// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"

#include "STUBaseWeapon.h"
#include "STUPickupBase.h"
#include "STUPickupAmmo.generated.h"

/**
 * 
 *
 */

DECLARE_LOG_CATEGORY_CLASS(LogPickupAmmo, Log, Log);

UCLASS()
class MYSHOOTTHEMUP_API ASTUPickupAmmo : public ASTUPickupBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="AmmoPickup")
	TSubclassOf<ASTUBaseWeapon> WeaponClass;
	
	UPROPERTY(EditAnywhere, Category="AmmoPickup", meta = (ClampMin="0", ClampMax="100"))
	int32 AmmoClips;

private:
	virtual bool GivePickupTo(APawn* Pawn) override;

};
