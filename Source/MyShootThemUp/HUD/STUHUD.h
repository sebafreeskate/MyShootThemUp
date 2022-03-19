// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYSHOOTTHEMUP_API ASTUHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="PlayerUI")
	TSubclassOf<UUserWidget> PlayerHudWidgetClass;

	virtual void BeginPlay() override;

private:
	void DrawCrossHair();
};
