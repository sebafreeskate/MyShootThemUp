// Nikita Kitov. All rights reserved


#include "MySTURiffleWeapon.h"

#include "DrawDebugHelpers.h"
#include "STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/PointLightComponent.h"

void ASTURiffleWeapon::DrawDebugSpreadCone() const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetViewLocationRotation(ViewLocation, ViewRotation)) return;
	DrawDebugCone(GetWorld(), GetMuzzleTransform().GetLocation(), ViewRotation.Vector(), 50.f
	              ,FMath::DegreesToRadians(CurrentSpreadHalfAngle)
	              ,FMath::DegreesToRadians(CurrentSpreadHalfAngle)
	              ,10.f, FColor::Green
	);
}

void ASTURiffleWeapon::UpdateSpread(float DeltaSeconds)
{
	if (bFiring)
	{
		TimeFiring += DeltaSeconds;
	}
	else
	{
		TimeFiring -= DeltaSeconds;
	}

	TimeFiring = FMath::Clamp(TimeFiring, 0.f, FireTimeForMaxSpread);
	CurrentSpreadHalfAngle = FMath::GetMappedRangeValueClamped({ 0.f, FireTimeForMaxSpread },
		                                                           {0.f, MaxSpreadHalfAngle}, TimeFiring);
	
	//DrawDebugSpreadCone();
}

void ASTURiffleWeapon::InitMuzzleFx()
{
	if (!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
		MuzzleFXComponent->SetVisibility(false);
	}
}

void ASTURiffleWeapon::SetMuzzleEffectVisible(bool Visible) const
{
	if(!MuzzleFXComponent || !MuzzleFlashPointLight)
	{
		return;
	}

	//MuzzleFXComponent->SetPaused(!Visible);
	MuzzleFXComponent->SetVisibility(Visible);
	MuzzleFlashPointLight->SetVisibility(Visible);
}

ASTURiffleWeapon::ASTURiffleWeapon()
{
	FXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("FXComponent");
	MuzzleFlashPointLight = CreateDefaultSubobject<UPointLightComponent>("MuzzlePointLight");
	MuzzleFlashPointLight->SetupAttachment(WeaponMesh, MuzzleSocketName);
	MuzzleFlashPointLight->SetVisibility(false);
}

void ASTURiffleWeapon::Tick(float DeltaSeconds)
{
	if(GetOwnerCharacter()->IsLocallyControlled())
	{
		UpdateSpread(DeltaSeconds);
	}

	UpdateSpread(DeltaSeconds);
}

void ASTURiffleWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(FXComponent);

	InitMuzzleFx();

}

void ASTURiffleWeapon::StartFire()
{
	Super::StartFire();

	if(!GetWorld()) return;

	MakeShoot();
	GetWorld()->GetTimerManager().SetTimer(FireHandle, this, &ASTURiffleWeapon::MakeShoot, TimeBetweenShoots, true);

	bFiring = true;
}

void ASTURiffleWeapon::StopFire()
{
	if(!GetWorld()) return;

	GetWorld()->GetTimerManager().ClearTimer(FireHandle);

	bFiring = false;
}

bool ASTURiffleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd, FVector& TraceDir) const
{
	FVector ViewLocation;
	FRotator ViewRotation;

	if(!GetViewLocationRotation(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	TraceDir = FMath::VRandCone(ViewRotation.Vector(), FMath::DegreesToRadians(CurrentSpreadHalfAngle));
	TraceEnd = TraceStart + TraceDir * TraceDistance;

	return true;
}

void ASTURiffleWeapon::SpawnTraceVFX(FVector TraceStart, FVector TraceEnd)
{
	auto SpawnedVFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceVFX, TraceStart);
	if(SpawnedVFX)
	{
		SpawnedVFX->SetNiagaraVariableVec3(TraceEndParamName, TraceEnd);
	}
}

void ASTURiffleWeapon::MakeMuzzleShootFX()
{
	SetMuzzleEffectVisible(true);
	FTimerDelegate TurnOffMuzzleEffectDelegate;
	TurnOffMuzzleEffectDelegate.BindUObject(this, &ASTURiffleWeapon::SetMuzzleEffectVisible, false);
	if(GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(MuzzleEffectTurnOffHandle, TurnOffMuzzleEffectDelegate, TimeBetweenShoots * 0.7, false);
	}
}

void ASTURiffleWeapon::MakeShoot()
{
	if(!GetWorld()) return;

	if(!CurrentAmmo.AmmoInClip)
	{
		StopFire();

		return;
	}
	
	FVector TraceStart, TraceEnd, TraceDir;
	if(!GetTraceData(TraceStart, TraceEnd, TraceDir)) return;

	FHitResult HitResult;
	MakeTraceHit(TraceStart, TraceEnd, HitResult);

	const float DebugLifeTime = 3.f;

	FVector VFXTraceEnd = TraceEnd;

	if(HitResult.bBlockingHit)
	{
		VFXTraceEnd = HitResult.ImpactPoint;
		
		FXComponent->SpawnImpactFX(HitResult);

		MakeDamage(HitResult);
	}

	SpawnTraceVFX(GetMuzzleTransform().GetLocation(), VFXTraceEnd);
	
	MakeMuzzleShootFX();

	//auto NewWeaponRotation = TraceDir.Rotation();
	//SetActorRotation(NewWeaponRotation.Add(0, -90, 0));
	//SetActorRotation(NewWeaponRotation);
	//DrawDebugLine(GetWorld(), GetMuzzleTransform().GetLocation(), GetMuzzleTransform().GetLocation()+NewWeaponRotation.Vector()* 200, FColor::Green, false, 2.f, 0, 3.f);
	//DrawDebugLine(GetWorld(), GetMuzzleTransform().GetLocation(), GetMuzzleTransform().GetLocation()+TraceDir.Rotation().Vector()* 200, FColor::Green, false, 2.f, 0, 3.f);
	
	DecreaseAmmo();
}


void ASTURiffleWeapon::MakeDamage(const FHitResult& HitResult)
{
	if (!HitResult.GetActor()) return;

	float DamageToMake = Damage;

	if (HitResult.BoneName == "b_Head")
	{
		DamageToMake *= HeadShootMultiplier;
	}

	HitResult.GetActor()->TakeDamage(DamageToMake, FDamageEvent{}, GetPlayerController(), GetOwner());
}

