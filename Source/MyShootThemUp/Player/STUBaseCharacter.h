// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class USTUWeaponComponent;
class ASTUBaseWeapon;
class USpringArmComponent;
class UCameraComponent;
class USTUHealthComponent;
class UTextRenderComponent;
class UAnimMontage;

UCLASS()
class MYSHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure)
	bool IsInSprint() const { return bInSprint && GetVelocity().SizeSquared() > 0.f; }

	UFUNCTION(BlueprintPure)
	float GetDirectionAngleDegrees() const;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USTUHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTextRenderComponent* TextRenderComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USTUWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly)
	FVector2D FallVelocityRange = FVector2D(1000.f, 2000.f);

	UPROPERTY(EditDefaultsOnly)
	FVector2D FallDamageRange = FVector2D(30.f, 100.f);

	float CorpseLifespan;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	bool CanSprint() const;
	void EnableSprint();
	void DisableSprint();
	void UpdateHealthText(float Health) const;

	UFUNCTION()
	void EnableRagdoll(USkeletalMeshComponent* SkeletalMesh);

	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void OnGroundLanded(const FHitResult& HitResult);
	
	bool bInSprint;
};
