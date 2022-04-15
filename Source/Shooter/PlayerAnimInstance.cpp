// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "PlayerCharacter.h"


void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto player = Cast<APlayerCharacter>(this->TryGetPawnOwner());

	if (player == nullptr) return;

	Speed = player->GetVelocity().Size();

	if ( player->Loadout == ELoadout::NoWeapon) return;
	
	// To set the appropriate hand position for the held weapon.
	CurrentWeaponType = player->CurrentWeapon->WeaponType;
	IsSwitchingWeapon = player->IsSwitchingWeapon;
}