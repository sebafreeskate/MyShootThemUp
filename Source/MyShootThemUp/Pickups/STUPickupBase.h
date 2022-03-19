// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUPickupBase.generated.h"

DECLARE_LOG_CATEGORY_CLASS(LogPickup, Log, Log);

class USphereComponent;
UCLASS()
class MYSHOOTTHEMUP_API ASTUPickupBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUPickupBase();

protected:
	virtual void BeginPlay() override;
	void MakeActive(bool bActive) const;
	void PickupWasTaken();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual bool GivePickupTo(APawn* Pawn);

	virtual void Respawn();

	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* SphereCollision;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Pickup")
	float RespawnTime = 5.f;

public:	
	virtual void Tick(float DeltaTime) override;

};
