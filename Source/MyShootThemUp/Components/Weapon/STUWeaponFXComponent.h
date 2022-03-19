// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"

#include "NiagaraCommon.h"
#include "STUCoreTypes.h"
#include "Components/ActorComponent.h"
#include "STUWeaponFXComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSHOOTTHEMUP_API USTUWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUWeaponFXComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnImpactFX(const FHitResult& HitResult) const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Hit Impact Effect")
	FImpactVFXData DefaultImpactVFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hit Impact Effect")
	TMap<UPhysicalMaterial*, FImpactVFXData> PhysMatImpactEffects;

};
