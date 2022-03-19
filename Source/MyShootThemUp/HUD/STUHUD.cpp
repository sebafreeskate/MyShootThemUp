// Nikita Kitov. All rights reserved


#include "STUHUD.h"


#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"

void ASTUHUD::DrawHUD()
{
	Super::DrawHUD();
	//DrawCrossHair();
}

void ASTUHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if(!PlayerHudWidgetClass)
	{
		return;
	}

	auto HUDWidget = CreateWidget<UUserWidget>( GetOwningPlayerController(), PlayerHudWidgetClass);

	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}
}

void ASTUHUD::DrawCrossHair()
{
	FVector2D Center;
	Canvas->GetCenter(Center.X, Center.Y);

	const float LineThickness = 4.f;
	const float LineHalhLength = 10.f;

	DrawLine(Center.X - LineHalhLength, Center.Y, Center.X + LineHalhLength,
		Center.Y, FLinearColor::Green, LineThickness);
	DrawLine(Center.X, Center.Y - LineHalhLength, Center.X, 
		Center.Y + LineHalhLength, FLinearColor::Green, LineThickness);
}
