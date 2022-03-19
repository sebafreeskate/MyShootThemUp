// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STUNextWeaponService.generated.h"

/**
 * 
 */
UCLASS()
class MYSHOOTTHEMUP_API USTUNextWeaponService : public UBTService
{
	GENERATED_BODY()

public:

	USTUNextWeaponService();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, Category = "AI")
	float Probability = 0.8f;
	
};
