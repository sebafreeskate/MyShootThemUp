// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWeapon.h"
#include "STULauncherWeapon.generated.h"

class ASTUProjectile;
/**
 * 
 */
UCLASS()
class MYSHOOTTHEMUP_API ASTULauncherWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASTUProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageType> DamageType = UDamageType::StaticClass();

	UPROPERTY(EditDefaultsOnly)
	bool DoFullDamage = false;

	UPROPERTY(EditDefaultsOnly)
	float ExplosionRadius = 200;

	virtual void StartFire() override;

	float GetOwnerCharacterMovementSpeed() const;

	virtual void MakeShoot() override;

};
