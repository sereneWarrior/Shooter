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
	UE_LOG(LogTemp, Warning, TEXT("Create Widget"));
	CrossHairWidget->AddToViewport();
	//CrossHairWidget->SetPositionInViewport(FVector2D(200, 200));
	//UE_LOG(LogTemp, Warning, TEXT("Create Widget"));
	CrossHairWidget->CrossHairUpdate();
}

void AMyHUD::UpdateCrossHairWidget()
{
	CrossHairWidget->CrossHairUpdate();
}
