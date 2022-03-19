// Nikita Kitov. All rights reserved


#include "STUProjectile.h"

#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "STUWeaponFXComponent.h"


ASTUProjectile::ASTUProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetSphereRadius(5.f);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SphereCollision->IgnoreActorWhenMoving(GetOwner(), true);
	SphereCollision->bReturnMaterialOnMove = true;
	SetRootComponent(SphereCollision);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	ProjectileMesh->SetupAttachment(SphereCollision);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->ProjectileGravityScale = 0.01;
	ProjectileMovement->InitialSpeed = 1000.f;

	FXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("FXComponent");

}

void ASTUProjectile::AddVelocity(float OwnerCharacterMovementSpeed) const
{
	ProjectileMovement->InitialSpeed += OwnerCharacterMovementSpeed;
}

void ASTUProjectile::SetOwningWeapon(ASTUBaseWeapon* Weapon)
{
	this->OwningWeapon = Weapon;
}

void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();

	check(ProjectileMovement);
	check(SphereCollision);
	check(FXComponent);

	ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;

	SetLifeSpan(5.f);
	SphereCollision->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);
}

void ASTUProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(WeaponLog, Log, TEXT("%s was hitted on %s's component %s"), *GetNameSafe(this), *GetNameSafe(OtherActor), *GetNameSafe(OtherComp));

	if(!GetWorld()) return;

	ProjectileMovement->StopMovementImmediately();

	const TArray<AActor*> IgnoredActors{this, GetOwner(), GetOwningWeapon()};

	FVector ImpactPoint = Hit.ImpactPoint;
	ImpactPoint.Z += 50.f;

	UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		Damage,
		ImpactPoint,
		Radius,
		DamageType,
		IgnoredActors,
		GetOwner(),
		GetOwnerPlayerController(),
		bDoFullDamage,
		ECollisionChannel::ECC_Pawn);

	//DrawDebugSphere(GetWorld(), ImpactPoint, Radius, 12, FColor::Red, false, 3.f);
	FXComponent->SpawnImpactFX(Hit);

	Destroy();
}

AController* ASTUProjectile::GetOwnerPlayerController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn? Pawn->GetController() : nullptr;
}
