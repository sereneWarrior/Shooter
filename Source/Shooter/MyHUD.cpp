// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"


void AMyHUD::DrawHUD()
{
	Super::DrawHUD();

	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	AHUD::DrawRect(FLinearColor::Black, Center.X, Center.Y, 4.0f, 4.0f);
}

void AMyHUD::BeginPlay()
{
	if (!CrossHairClass){return;}
	CrossHairWidget = CreateWidget<UCrossHair>(GetWorld(), CrossHairClass);
	if (!CrossHairWidget) { return; }

	CrossHairWidget->AddToViewport();
	CrossHairWidget->CrossHairUpdate();

	WeaponInfoWidget = CreateWidget<UWeaponInfoWidget>(GetWorld(), WeaponInfoClass);
	if (!WeaponInfoWidget) return;

	WeaponInfoWidget->AddToViewport();
	//WeaponInfoWidget->SetCurrentAmmoValue();
}

void AMyHUD::UpdateCrossHairWidget()
{
	CrossHairWidget->CrossHairUpdate();
}
