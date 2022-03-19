// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "Camera/CameraShakeBase.h"

#include "STUHealthComponent.generated.h"

class AActor;
class UDamageType;
class AController;
class UCameraShake;

DECLARE_LOG_CATEGORY_EXTERN(LogHealthComponent, All, All)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USTUHealthComponent();
	void UpdateLastDamageTime();

	UFUNCTION(BlueprintPure)
		float GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure)
		float GetHealthPercent() const { return Health / DefaultHealth; }

	UFUNCTION(BlueprintPure)
		bool IsDead() const { return FMath::IsNearlyZero(Health); }

	bool TryHeal(float HealAmount);

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	FOnHealthChanged OnHealthChanged;

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
		float DefaultHealth = 100;

	UPROPERTY(EditDefaultsOnly, Category = "Regeneration")
		bool bRegen = true;

	UPROPERTY(EditDefaultsOnly, Category = "Regeneration", meta = (EditCondition = "bRegen"))
		float RegenValuePerSecond = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Regeneration", meta = (EditCondition = "bRegen"))
		float DelayBeforeRegenerationStarts = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
		TSubclassOf<UCameraShakeBase> DamageCameraShake;

private:

	UFUNCTION()
		void OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
			class AController* InstigatedBy, AActor* DamageCauser);

	void PlayCameraShake() const;
		
	FTimerHandle HandleRegeneration;

	void SetNewHealth(float NewHealth);

	float Health = 0.f;

	float LastDamageTime;
};
