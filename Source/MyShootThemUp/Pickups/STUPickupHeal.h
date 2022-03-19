// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "STUPickupBase.h"
#include "STUPickupHeal.generated.h"

/**
 * 
 */
DECLARE_LOG_CATEGORY_CLASS(LogPickupHeal, Log, Log);

UCLASS()
class MYSHOOTTHEMUP_API ASTUPickupHeal : public ASTUPickupBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category="Heal", meta = (ClampMin="0", ClampMax="100"))
	float HealAmount;

private:
	virtual bool GivePickupTo(APawn* Pawn) override;
};
