// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponInfoWidget.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

UWeaponInfoWidget::UWeaponInfoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}


void UWeaponInfoWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);

	// TODO: Check cross hair visibility status
	if (Player->Loadout == ELoadout::HasGun)
	{
		Panel->SetVisibility(ESlateVisibility::Visible);
		SetCurrentAmmoValue();
		return;
	}

	Panel->SetVisibility(ESlateVisibility::Collapsed);
	
}

void UWeaponInfoWidget::SetCurrentAmmoValue() 
{
	// TODO: Move logic to upper level? Use blueo
	if (Player->Loadout == ELoadout::NoWeapon)
	{
		return;
	}
	auto currentWeapon = Player->CurrentWeapon;
	AmmoCount->SetText(FText::AsNumber(currentWeapon->CurrentAmmoCount));
	TotalAmmoCount->SetText(FText::AsNumber(currentWeapon->CurrentTotalAmmo));

	Type->SetText(currentWeapon->AmmoType);
}