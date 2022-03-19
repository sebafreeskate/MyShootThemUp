// Nikita Kitov. All rights reserved


#include "STUBaseWeapon.h"

#include "STUBaseCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/KismetSystemLibrary.h"

ASTUBaseWeapon::ASTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::StartFire()
{
	CheckClipForEmptiness(this);
}

ASTUBaseCharacter* ASTUBaseWeapon::GetOwnerCharacter() const
{
	return Cast<ASTUBaseCharacter>(GetOwner());
}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = DefaultAmmo;

	check(WeaponMesh);
	checkf(DefaultAmmo.AmmoInClip > 0, TEXT("Bullets count could be less or equal zero"));
	checkf(DefaultAmmo.Clips > 0, TEXT("Clips count could be less or equal zero"));

}

void ASTUBaseWeapon::MoveHittedTarget(const FTransform MuzzleTransform, FHitResult HitResult) const
{
	if(HitResult.Actor->IsValidLowLevelFast() && HitResult.Actor->IsRootComponentMovable())
	{
		const FVector MuzzleDirection = MuzzleTransform.GetRotation().Vector();
		const FVector HitOffset = MuzzleDirection
			* FVector::DotProduct( GetOwner()->GetVelocity().GetSafeNormal(), (MuzzleDirection.GetSafeNormal()))
			* 100.f;
		HitResult.Actor->AddActorWorldOffset(MuzzleTransform.GetRotation().Vector() * HitOffset);
	}
}

FTransform ASTUBaseWeapon::GetMuzzleTransform() const
{
	return WeaponMesh->GetSocketTransform(MuzzleSocketName);
}

AController* ASTUBaseWeapon::GetPlayerController() const
{
	const auto OwnerCharacter = GetOwnerCharacter();

	if(!GetOwnerCharacter()) return nullptr;

	return OwnerCharacter->GetController();
}

bool ASTUBaseWeapon::GetViewLocationRotation(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto Controller = GetPlayerController();
	if(!Controller) return false;
	if(Controller->GetPawn() && Controller->GetPawn()->IsPlayerControlled())
	{
		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}
	else
	{
		ViewLocation = GetMuzzleTransform().GetLocation();
		ViewRotation = GetMuzzleTransform().GetRotation().Rotator();
	}

	return true;
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd, FVector& TraceDir) const
{
	FVector ViewLocation;
	FRotator ViewRotation;

	if(!GetViewLocationRotation(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	TraceEnd = TraceStart + FRotationMatrix(ViewRotation).GetScaledAxis(EAxis::X) * TraceDistance;

	return true;
}

void ASTUBaseWeapon::MakeTraceHit(const FVector& TraceStart, const FVector& TraceEnd, FHitResult& HitResult) const
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.bReturnPhysicalMaterial = true;

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd,
	                                     ECollisionChannel::ECC_Visibility, QueryParams);
}

/*           Ammo shit             */

void ASTUBaseWeapon::DecreaseAmmo()
{
	CurrentAmmo.AmmoInClip--;
	
	if (CurrentAmmo.AmmoInClip == 0)
	{
		OnClipIsEmpty.Broadcast(this);
	}

	UE_LOG(WeaponLog, Log, TEXT("Current ammo = %d, Clips = %d"), CurrentAmmo.AmmoInClip, CurrentAmmo.Clips);
}

bool ASTUBaseWeapon::CanReload() const
{
	return CurrentAmmo.Clips > 0 || CurrentAmmo.bInfinite;
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 AmmoClips)
{
	if(CurrentAmmo.AmmoInClip == DefaultAmmo.AmmoInClip 
		&& CurrentAmmo.Clips == DefaultAmmo.Clips)
	{
		return false;
	}
	
	if(CurrentAmmo.bInfinite)
	{
		return false;
	}

	const int32 NewClipsValue = FMath::Min(CurrentAmmo.Clips + AmmoClips, DefaultAmmo.Clips);
	if(CurrentAmmo.AmmoInClip == 0)
	{
		CurrentAmmo.Clips = NewClipsValue + 1;		
		OnClipIsEmpty.Broadcast(this);
	}
	else
	{
		CurrentAmmo.Clips = NewClipsValue;		
	}

	return true;
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.AmmoInClip && !CurrentAmmo.Clips;
}

FVector ASTUBaseWeapon::GetSecondHandSocketLocation() const
{
	return WeaponMesh->GetSocketLocation(SecondHandSocketName);
}

void ASTUBaseWeapon::Reload()
{
	if(CanReload())
	{
		ChangeAmmoClip();
	}
}

void ASTUBaseWeapon::ChangeAmmoClip()
{
	CurrentAmmo.AmmoInClip = DefaultAmmo.AmmoInClip;
	CurrentAmmo.Clips--;

	UE_LOG(WeaponLog, Log, TEXT("Current ammo = %d, Clips = %"), CurrentAmmo.AmmoInClip, CurrentAmmo.Clips);
}

void ASTUBaseWeapon::CheckClipForEmptiness(ASTUBaseWeapon*)
{
	if (!CurrentAmmo.AmmoInClip)
	{
		OnClipIsEmpty.Broadcast(this);
	}
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX() 
{
	const auto SpawnedSystem = UNiagaraFunctionLibrary::SpawnSystemAttached(
		  MuzzleFX
		, WeaponMesh
		, MuzzleSocketName
		, FVector::ZeroVector
		, FRotator::ZeroRotator
		, EAttachLocation::SnapToTarget
		, true
	);

	SpawnedSystem->SetTickGroup(PrimaryActorTick.TickGroup);
	SpawnedSystem->AddTickPrerequisiteActor(this);

	return SpawnedSystem;
}

