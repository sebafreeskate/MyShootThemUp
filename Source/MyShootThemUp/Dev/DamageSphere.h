// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageSphere.generated.h"

UCLASS()
class MYSHOOTTHEMUP_API ADamageSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageSphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	USceneComponent* SceneComponent;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Radius = 300.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FColor DebugColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool DoFullDamage = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Damage = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UDamageType> DamageType;
};
