// Nikita Kitov. All rights reserved


#include "STUPlayerHUDWidget.h"

#include "STUHealthComponent.h"
#include "STUWeaponComponent.h"
#include "STUUtils.h"

bool USTUPlayerHUDWidget::Initialize()
{
	const auto HealthComp = GetHealthComponnent();

	if(HealthComp)
	{
		HealthComp->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChangedEvent);		
	}
	
	return Super::Initialize();
}

float USTUPlayerHUDWidget::GetHealthPercent()
{
	const auto Player = GetOwningPlayer();

	if(!Player || !Player->GetPawn())
	{
		return 0.f;
	}

	const auto Comp= Player->GetPawn()->GetComponentByClass(USTUHealthComponent::StaticClass());
	const auto HealthComp = Cast<USTUHealthComponent>(Comp);

	if(!HealthComp)
	{
		return 0.f;
	}

	return HealthComp->GetHealthPercent();
}

USTUWeaponComponent* USTUPlayerHUDWidget::GetWeaponComponent() const
{
	return GetPawnComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
}

USTUHealthComponent* USTUPlayerHUDWidget::GetHealthComponnent() const
{
	return GetPawnComponent<USTUHealthComponent>(GetOwningPlayerPawn());
}

FUIWeaponData USTUPlayerHUDWidget::GetWeaponUIData()
{
	FUIWeaponData CurrentWeaponData = {};

	USTUWeaponComponent* WeaponComponent = GetWeaponComponent();
	if(!WeaponComponent)
	{
		return CurrentWeaponData;
	}

	WeaponComponent->GetWeaponUIData(CurrentWeaponData);

	return CurrentWeaponData;
}

FAmmoData USTUPlayerHUDWidget::GetWeaponAmmoData()
{
	FAmmoData CurrentAmmoData = {};

	USTUWeaponComponent* WeaponComponent = GetWeaponComponent();
	if(!WeaponComponent)
	{
		return CurrentAmmoData;
	}

	WeaponComponent->GetCurrentAmmoData(CurrentAmmoData);

	return CurrentAmmoData;
}

bool USTUPlayerHUDWidget::IsAlive()
{
	return GetHealthComponnent() && !GetHealthComponnent()->IsDead();
}

bool USTUPlayerHUDWidget::IsSpectating()
{
	return GetOwningPlayer() && GetOwningPlayer()->GetStateName() == NAME_Spectating;
}
