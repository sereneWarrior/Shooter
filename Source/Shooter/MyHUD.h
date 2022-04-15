// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Shooter/CrossHair.h"
#include <Shooter/WeaponInfoWidget.h>
#include "Components/WidgetComponent.h"

#include "MyHUD.generated.h"

UCLASS()
class SHOOTER_API AMyHUD : public AHUD
{
	GENERATED_BODY()


public:
	virtual void DrawHUD() override;
	virtual void BeginPlay() override;

	void UpdateCrossHairWidget();
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UUserWidget> CrossHairClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UUserWidget> WeaponInfoClass;

private:
	UCrossHair* CrossHairWidget;

	UWeaponInfoWidget* WeaponInfoWidget;
};
