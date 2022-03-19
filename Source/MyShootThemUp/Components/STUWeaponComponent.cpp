// Nikita Kitov. All rights reserved


#include "STUWeaponComponent.h"

#include "AnimUtills.h"
#include "STUAnimNotifyReload.h"
#include "STUEquipFinishedAnimNotify.h"
#include "STUBaseWeapon.h"
#include "GameFramework/Character.h"

constexpr static int32 MaxWeaponsNum = 2;

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::StartFire() 
{
	if(!CurrentWeapon || !CanFire()) return;

	CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire() 
{
	if(!CurrentWeapon) return;

	CurrentWeapon->StopFire();
}

void USTUWeaponComponent::OnReloadPressed()
{
	Reload(CurrentWeapon);
}

void USTUWeaponComponent::PlayReloadAnimation()
{
	bReloadInProgress = true;
	PlayAnimMontage(CurrentReloadAnimation);
	StopFire();
}

void USTUWeaponComponent::Reload(ASTUBaseWeapon* WeaponToReload) 
{
	if(!WeaponToReload)
	{
		UE_LOG(LogWeaponComponent, Warning, TEXT("WeaponToReload is nullptr"));
		return;
	}
	
	if(!CanReload()) return;

	if(CurrentWeapon == WeaponToReload)
	{
		PlayReloadAnimation();
	}
}

void USTUWeaponComponent::DoChangeWeapon(USkeletalMeshComponent* CharacterMesh)
{
	const auto OwnerCharacter = Cast<ACharacter>(GetOwner());
	if(!OwnerCharacter) return;
	if(OwnerCharacter->GetMesh() != CharacterMesh) return;

	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();

	CurrentWeapon->StopFire();
	AttachWeaponToSocket(CurrentWeapon, ArmorySocketName);

	AttachWeaponToSocket(CurrentWeapon, WeaponSocketName);

	bEquipInProgress = false;

	SetCurrentWeapon(Weapons[CurrentWeaponIndex]);
}

void USTUWeaponComponent::DoReloadWeapon(USkeletalMeshComponent* CharacterMesh)
{
	const auto OwnerCharacter = Cast<ACharacter>(GetOwner());
	if(!OwnerCharacter) return;
	if(OwnerCharacter->GetMesh() != CharacterMesh) return;

	CurrentWeapon->Reload();

	bReloadInProgress = false;
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* AnimToPlay) const
{
	const auto OwnerCharacter = Cast<ACharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->PlayAnimMontage(AnimToPlay);
}

void USTUWeaponComponent::NextWeapon()
{
	PlayAnimMontage(EquipAnimation);
	bEquipInProgress = true;
	bReloadInProgress = false;
}

void USTUWeaponComponent::SetupAnimations()
{
	USTUEquipFinishedAnimNotify* EquipNotify = AnimUtills::FindAnimNotify<USTUEquipFinishedAnimNotify>(EquipAnimation);
	if(EquipNotify)
	{
		EquipNotify->STUNotifyDelegate.AddDynamic(this, &USTUWeaponComponent::DoChangeWeapon);
	}
	else
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify not found!"));
		checkNoEntry();
	}

	for (auto WeaponData : WeaponsData)
	{
		USTUAnimNotifyReload* ReloadNotify = AnimUtills::FindAnimNotify<USTUAnimNotifyReload>(WeaponData.ReloadAnimation);
		if (ReloadNotify)
		{
			ReloadNotify->STUNotifyDelegate.AddDynamic(this, &USTUWeaponComponent::DoReloadWeapon);
		}
		else
		{
			UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify for %s not found!"), *GetNameSafe(WeaponData.WeaponClass));
			checkNoEntry();
		}
	}
}

bool USTUWeaponComponent::CanFire() const
{
	const auto OwnerCharacter = Cast<ASTUBaseCharacter>(GetOwner());

	bool bIsInSprint = false;
	if(OwnerCharacter)
	{
		bIsInSprint = OwnerCharacter->IsInSprint();	
	}

	return CurrentWeapon && !bEquipInProgress && !bReloadInProgress && !bIsInSprint;
}

bool USTUWeaponComponent::CanReload() const
{
	return CanFire() && CurrentWeapon->CanReload();
}

bool USTUWeaponComponent::CanEquip() const
{
	return !bEquipInProgress && !bReloadInProgress;
}

bool USTUWeaponComponent::GetWeaponUIData(FUIWeaponData& WeaponUIData) const
{
	if (CurrentWeapon)
	{
		WeaponUIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}

bool USTUWeaponComponent::GetCurrentAmmoData(FAmmoData & AmmoData) const
{
	if(CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetCurrentAmmoData();

		return true;
	}
	return false;
}

bool USTUWeaponComponent::TryToAddAmmo(const TSubclassOf<ASTUBaseWeapon> WeaponClass, const int32 AmmoClips)
{
	for (auto Weapon : Weapons)
	{
		if (Weapon->IsA(WeaponClass))
		{
			if (!Weapon)
			{
				UE_LOG(LogWeaponComponent, Warning, TEXT("CurrentWeapon not found!"));
				return false;
			}
			return Weapon->TryToAddAmmo(AmmoClips);
		}
	}

	return false;
}

FVector USTUWeaponComponent::GetWeaponSecondHandSocketLocation() const
{
	return CurrentWeapon->GetSecondHandSocketLocation();
}

void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponsData.Num() == MaxWeaponsNum, TEXT("Character can have only %d weapons equipped"), MaxWeaponsNum);
	InitWeapons();
	SetupAnimations();
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for(auto Weapon : Weapons)
	{
		if(!Weapon) continue;

		Weapon->StopFire();
		FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, false);
		Weapon->DetachFromActor(DetachmentRules);
		Weapon->Destroy();
	}

	Weapons.Empty();
	
	Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, FName SocketName) const
{
	const auto OwnerCharacter = Cast<ACharacter>(GetOwner());

	if(!OwnerCharacter) return;

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(OwnerCharacter->GetMesh(), AttachmentRules, SocketName);
	Weapon->SetOwner(OwnerCharacter);
}

void USTUWeaponComponent::SetCurrentWeaponIndex(int32 WeaponIndex)
{
	CurrentWeaponIndex = WeaponIndex;
	if (Weapons.Num() > CurrentWeaponIndex)
	{
		SetCurrentWeapon(Weapons[CurrentWeaponIndex]);
	}
}

void USTUWeaponComponent::SetCurrentWeapon(ASTUBaseWeapon* WeaponToEquip)
{
	if(CurrentWeapon)
	{
		AttachWeaponToSocket(CurrentWeapon, ArmorySocketName);
	}

	CurrentWeapon = WeaponToEquip;

	AttachWeaponToSocket(CurrentWeapon, WeaponSocketName);

	UpdateCurrentWeaponReloadAnimation();
	
	OnWeaponEquipped.Broadcast(CurrentWeapon);
}

bool USTUWeaponComponent::AddNewWeapon(FWeaponData WeaponData)
{
	auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponData.WeaponClass);
	if (!Weapon) return true;

	Weapons.Add(Weapon);
	AttachWeaponToSocket(Weapon, ArmorySocketName);
	Weapon->OnClipIsEmpty.AddUObject(this, &USTUWeaponComponent::Reload);
	OnWeaponEquipped.AddUObject(Weapon, &ASTUBaseWeapon::CheckClipForEmptiness);
	return false;
}

void USTUWeaponComponent::UpdateCurrentWeaponReloadAnimation()
{
	const auto CurrentWeaponData = WeaponsData.FindByPredicate([&](const FWeaponData& WeaponData)
	{
		return WeaponData.WeaponClass == CurrentWeapon->GetClass(); 
	});
	CurrentReloadAnimation = CurrentWeaponData->ReloadAnimation;
}

void USTUWeaponComponent::InitWeapons()
{

	if(!GetWorld()) return;

	for(auto WeaponData: WeaponsData)
	{
		if (AddNewWeapon(WeaponData)) continue;
	}

	SetCurrentWeapon(Weapons[CurrentWeaponIndex]);
}

