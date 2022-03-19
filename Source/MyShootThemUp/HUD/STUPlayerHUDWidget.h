// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"

#include "STUCoreTypes.h"
#include "STUHealthComponent.h"
#include "STUWeaponComponent.h"
#include "Blueprint/UserWidget.h"
#include "STUPlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:	

	virtual bool Initialize() override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="UI")
		float GetHealthPercent();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="UI")
		FUIWeaponData GetWeaponUIData();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="UI")
		FAmmoData GetWeaponAmmoData();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="UI")
	bool IsAlive();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="UI")
	bool IsSpectating();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="UI")
	USTUHealthComponent* GetHealthComponnent() const;

	USTUWeaponComponent* GetWeaponComponent() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnHealthChangedEvent(float Delta);
};
