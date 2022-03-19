#pragma once
#include "STUCoreTypes.generated.h"

class UMaterialInterface;
class UNiagaraSystem;
class ASTUBaseWeapon;

//Weapon
DECLARE_MULTICAST_DELEGATE_OneParam(FClipIsEmpty, ASTUBaseWeapon*);
DECLARE_MULTICAST_DELEGATE_OneParam(FWeaponEquipped, ASTUBaseWeapon*);

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon" )
	int32 AmmoInClip;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon", meta = (EditCondittion = "!bInfinite"))
	int32 Clips;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	bool bInfinite;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<ASTUBaseWeapon> WeaponClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UAnimMontage* ReloadAnimation;
};

USTRUCT(BlueprintType)
struct FUIWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="UI")
	UTexture2D* MainIcon;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="UI")
	UTexture2D* CrosshairIcon;
};

//Health
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);


//Impacts

USTRUCT(BlueprintType)
struct FDecailData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Decail")
	float LifeTime = 7.f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Decail")
	float FadeTime = 0.7f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Decail")
	FVector Size = FVector(10.f);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Decail")
	UMaterialInterface* Material;
};

USTRUCT(BlueprintType)
struct FImpactVFXData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Particles")
	UNiagaraSystem* ImpactParticles;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Decail")
	FDecailData DecailData;
};
