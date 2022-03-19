// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"


#include "STUBaseCharacter.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"

#include "STUBaseWeapon.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
DECLARE_LOG_CATEGORY_CLASS(WeaponLog, All, All);

class USkeletalMeshComponent;

UCLASS()
class MYSHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUBaseWeapon();

	virtual void StartFire();

	virtual void StopFire(){}

	FVector GetSecondHandSocketLocation() const;

	void Reload();

	bool CanReload() const;

	bool TryToAddAmmo(int32 AmmoClips);

	bool IsAmmoEmpty() const;

	ASTUBaseCharacter* GetOwnerCharacter() const;

	FUIWeaponData GetUIData() const { return UIData; }

	FAmmoData GetCurrentAmmoData() const { return CurrentAmmo;}

	void CheckClipForEmptiness(ASTUBaseWeapon* /*not used*/);

	FClipIsEmpty OnClipIsEmpty;

protected:

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly)
	FName MuzzleSocketName = "MuzzleFlashSocket";

	UPROPERTY(EditDefaultsOnly)
	FName SecondHandSocketName = "SecondHandSocket";

	UPROPERTY(EditDefaultsOnly)
	float TraceDistance = 20000.f;

	UPROPERTY(EditDefaultsOnly)
	float Damage = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float HeadShootMultiplier = 10.f;

	UPROPERTY(EditDefaultsOnly)
	FAmmoData DefaultAmmo;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	FUIWeaponData UIData;

	UPROPERTY(EditDefaultsOnly, Category="VFX")
	UNiagaraSystem* MuzzleFX;

	FAmmoData CurrentAmmo;

	virtual void BeginPlay() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector&, FVector& TraceDir) const;
	virtual void MakeShoot() PURE_VIRTUAL(ASTUBaseWeapon::MakeShoot);
	virtual void MakeDamage(const FHitResult& HitResult) PURE_VIRTUAL(STUBaseWeapon::MakeDamage);

	void MoveHittedTarget(FTransform MuzzleTransform, FHitResult HitResult) const;
	bool GetViewLocationRotation(FVector& ViewLocation, FRotator& ViewRotation) const;
	void MakeTraceHit(const FVector& TraceStart, const FVector& TraceEnd, FHitResult& HitResult) const;
	FTransform GetMuzzleTransform() const;
	AController* GetPlayerController() const;

	void DecreaseAmmo();
	void ChangeAmmoClip();

	UNiagaraComponent* SpawnMuzzleFX(); 

private:
	

};
