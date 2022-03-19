// Nikita Kitov. All rights reserved


#include "DamageSphere.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADamageSphere::ADamageSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
}

// Called when the game starts or when spawned
void ADamageSphere::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADamageSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, DebugColor);

	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage * DeltaTime, GetActorLocation(),
		Radius, DamageType, {}, this, nullptr, DoFullDamage);
}

