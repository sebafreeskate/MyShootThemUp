// Nikita Kitov. All rights reserved


#include "STULauncherWeapon.h"

#include "DrawDebugHelpers.h"
#include "STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void ASTULauncherWeapon::StartFire()
{
	Super::StartFire();
	MakeShoot();
}

float ASTULauncherWeapon::GetOwnerCharacterMovementSpeed() const
{
	if(const auto Char = GetOwnerCharacter())
	{
		if(Char->GetMovementComponent())
		{
			return Char->GetMovementComponent()->Velocity.Size();
		}
	}
	return 0.f;
}

void ASTULauncherWeapon::MakeShoot()
{
	if(!GetWorld() || !ProjectileClass || !CurrentAmmo.AmmoInClip) return;

	FVector TraceStart, TraceEnd, TraceDir;
	if(!GetTraceData(TraceStart, TraceEnd, TraceDir)) return;

	FHitResult HitResult;
	MakeTraceHit(TraceStart, TraceEnd, HitResult);

	const auto MuzzleTransform = GetMuzzleTransform();

	auto Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, MuzzleTransform, this);
	if (Projectile)
	{
		//set params
		auto StartPoint = MuzzleTransform.GetLocation();
		
		auto EndPoint = HitResult.bBlockingHit? HitResult.ImpactPoint : TraceEnd;

		auto Direction = (EndPoint-StartPoint).GetSafeNormal();
		
		Projectile->SetOwner(GetOwner());
		Projectile->SetOwningWeapon(this);
		Projectile->SetShootDirection(Direction);
		Projectile->SetDamage(Damage);
		Projectile->SetRadius(ExplosionRadius);
		Projectile->AddVelocity(GetOwnerCharacterMovementSpeed());
		
		Projectile->FinishSpawning(MuzzleTransform);
	}

	DecreaseAmmo();
	SpawnMuzzleFX();
}

	
