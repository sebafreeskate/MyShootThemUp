// Nikita Kitov. All rights reserved


#include "STUPickupBase.h"

#include "Components/SphereComponent.h"

ASTUPickupBase::ASTUPickupBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetSphereRadius(50.f);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(SphereCollision);

}

void ASTUPickupBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASTUPickupBase::MakeActive(bool bActive) const
{
	SphereCollision->SetCollisionResponseToAllChannels(bActive? ECollisionResponse::ECR_Overlap : ECollisionResponse::ECR_Ignore);
	GetRootComponent()->SetVisibility(bActive, true);
}

void ASTUPickupBase::PickupWasTaken()
{
	MakeActive(false);

	FTimerHandle RespawnHandle;
	GetWorldTimerManager().SetTimer(RespawnHandle,this, &ASTUPickupBase::Respawn, RespawnTime);
}

void ASTUPickupBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	UE_LOG(LogPickup, Log, TEXT("%s Overlapped %s"), *GetNameSafe(OtherActor), *GetNameSafe(this));

	const auto Pawn = Cast<APawn>(OtherActor);
	if(GivePickupTo(Pawn))
	{
		PickupWasTaken();
	}
}

bool ASTUPickupBase::GivePickupTo(APawn* Pawn)
{
	return false;
}

void ASTUPickupBase::Respawn()
{
	MakeActive(true);
}

void ASTUPickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

