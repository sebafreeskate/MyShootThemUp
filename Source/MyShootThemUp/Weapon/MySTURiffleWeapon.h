// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWeapon.h"
#include "STUWeaponFXComponent.h"



#include "MySTURiffleWeapon.generated.h"

class UPointLightComponent;
//class USTUWeaponFXComponent;
/**
 * 
 */
UCLASS()
class MYSHOOTTHEMUP_API ASTURiffleWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()

public:

	ASTURiffleWeapon();
	
protected:

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	virtual void StartFire() override;
	virtual void StopFire() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "VFX")
	USTUWeaponFXComponent* FXComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* TraceVFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UPointLightComponent * MuzzleFlashPointLight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FString TraceEndParamName = "TraceEnd";

	UPROPERTY(EditDefaultsOnly)
	float TimeBetweenShoots = 0.1f;

	UPROPERTY(EditDefaultsOnly)
	float MaxSpreadHalfAngle = 3.f;

	UPROPERTY(EditDefaultsOnly)
	float FireTimeForMaxSpread = 2.f;

	UPROPERTY(EditDefaultsOnly)
	float SpreadFullCooldown = 2.f;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd, FVector& TraceDir) const override;

private:

	virtual void MakeShoot() override;
	virtual void MakeDamage(const FHitResult& HitResult) override;

	void DrawDebugSpreadCone() const;
	void UpdateSpread(float DeltaSeconds);

	void InitMuzzleFx();

	void SetMuzzleEffectVisible(bool Visible) const;

	void SpawnTraceVFX(FVector TraceStart, FVector TraceEnd);

	void MakeMuzzleShootFX();

	FTimerHandle FireHandle;

	FTimerHandle MuzzleEffectTurnOffHandle;

	float CurrentSpreadHalfAngle = 0.f;

	bool bFiring;

	float TimeFiring;

	UPROPERTY()
	UNiagaraComponent* MuzzleFXComponent;
};
