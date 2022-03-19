// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "STUWeaponComponent.h"
#include "STUAIWeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYSHOOTTHEMUP_API USTUAIWeaponComponent : public USTUWeaponComponent
{
	GENERATED_BODY()
	
public:
	virtual void StartFire() override;
	virtual void NextWeapon() override;
};
