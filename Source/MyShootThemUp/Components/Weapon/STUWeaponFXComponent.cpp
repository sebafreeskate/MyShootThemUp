// Nikita Kitov. All rights reserved


#include "STUWeaponFXComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::SpawnImpactFX(const FHitResult& HitResult) const
{
	auto Effect = DefaultImpactVFX;

	if(const auto PhysMat = HitResult.PhysMaterial.Get())
	{
		if(PhysMatImpactEffects.Contains(PhysMat))
		{
			Effect = PhysMatImpactEffects[PhysMat];
		}
	}
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect.ImpactParticles, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());

	auto SpawnedDecail =  UGameplayStatics::SpawnDecalAtLocation(GetWorld()
		, Effect.DecailData.Material
		, Effect.DecailData.Size
		, HitResult.Location
		, HitResult.Normal.Rotation()
		, Effect.DecailData.LifeTime);

	if(SpawnedDecail)
	{
		SpawnedDecail->SetFadeOut(Effect.DecailData.LifeTime, Effect.DecailData.FadeTime);
	}
}

void USTUWeaponFXComponent::BeginPlay()
{
	Super::BeginPlay();

}

void USTUWeaponFXComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

