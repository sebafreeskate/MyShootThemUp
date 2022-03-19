// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"

#include "STUWeaponComponent.generated.h"

DECLARE_LOG_CATEGORY_CLASS(LogWeaponComponent, Log, Log);

class USTUAnimNotifyBase;
class ASTUBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUWeaponComponent();

	virtual void StartFire();

	void StopFire();

	void OnReloadPressed();

	void PlayReloadAnimation();

	void Reload(ASTUBaseWeapon* WeaponToReload);

	virtual void NextWeapon();

	UFUNCTION(BlueprintPure)
	bool CanFire() const;

	bool CanReload() const;

	bool CanEquip() const;

	bool GetWeaponUIData(FUIWeaponData& WeaponUIData) const;

	bool GetCurrentAmmoData(FAmmoData& AmmoData) const;

	bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponClass, int32 AmmoClips);

	UFUNCTION(BlueprintPure)
	FVector GetWeaponSecondHandSocketLocation() const;

	FWeaponEquipped OnWeaponEquipped;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, FName SocketName) const;

	void SetCurrentWeaponIndex(int32 WeaponIndex);

	void SetCurrentWeapon(ASTUBaseWeapon* WeaponToEquip);

	bool AddNewWeapon(FWeaponData WeaponData);
	void UpdateCurrentWeaponReloadAnimation();

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TArray<FWeaponData> WeaponsData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		FName WeaponSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		FName ArmorySocketName = "ArmorySocket";

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* EquipAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* CurrentReloadAnimation;

	UPROPERTY()
	TArray<ASTUBaseWeapon*> Weapons;

	UPROPERTY()
	ASTUBaseWeapon* CurrentWeapon;

	int CurrentWeaponIndex = 0;

private:

	void InitWeapons();

	void SetupAnimations();

	UFUNCTION()
	void DoChangeWeapon(USkeletalMeshComponent* CharacterMesh);

	UFUNCTION()
	void DoReloadWeapon(USkeletalMeshComponent* CharacterMesh);

	void PlayAnimMontage(UAnimMontage* AnimToPlay) const;

	bool bEquipInProgress = false;
	bool bReloadInProgress = false;
};


