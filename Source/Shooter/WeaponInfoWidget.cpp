// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponInfoWidget.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

UWeaponInfoWidget::UWeaponInfoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void UWeaponInfoWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	SetCurrentAmmoValue();
}

void UWeaponInfoWidget::SetCurrentAmmoValue() 
{
	// TODO: Move logic to upper level?
	if (Player->Loadout == ELoadout::NoWeapon)
	{
		return;
	}
	AmmoCount->SetText(FText::FromString("Random String"));
	//Player->CurrentWeapon->CurrentAmmo;
}