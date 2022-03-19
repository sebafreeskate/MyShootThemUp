// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"



#include "STULauncherWeapon.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"

class USTUWeaponFXComponent;
class UProjectileMovementComponent;
class USphereComponent;
UCLASS()
class MYSHOOTTHEMUP_API ASTUProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		USphereComponent* SphereCollision;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		USTUWeaponFXComponent* FXComponent;


	ASTUProjectile();

	void SetShootDirection(FVector Direction) {ShootDirection = Direction;}

	void SetDamage(float damage) {this->Damage = damage;}

	void SetRadius(float radius) {this->Radius = radius;}

	void SetFullDamage(bool doFullDamage) {this->bDoFullDamage = doFullDamage;}

	void AddVelocity(float OwnerCharacterMovementSpeed) const;

	void SetOwningWeapon(ASTUBaseWeapon* OwningWeapon);

	ASTUBaseWeapon* GetOwningWeapon() const { return  OwningWeapon; }

protected:
	virtual void BeginPlay() override;

	FVector ShootDirection;

	float Damage;

	float Radius;

	bool bDoFullDamage = false;

	UPROPERTY()
	ASTUBaseWeapon* OwningWeapon;

	UFUNCTION()
	void OnProjectileHit(
		UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

private:

	AController* GetOwnerPlayerController() const;
};
