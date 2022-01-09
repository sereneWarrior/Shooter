// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "PlayerCharacter.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	Speed = 0.0f;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	Player = Cast<APlayerCharacter>(this->TryGetPawnOwner());
	if (Player == nullptr) return;
	Speed = Player->GetVelocity().Size();

	if ( Player->Loadout == ELoadout::NoWeapon) return;

	CurrentWeaponType = Player->CurrentWeapon->WeaponType;
	IsSwitchingWeapon = Player->IsSwitchingWeapon;
	//UE_LOG(LogTemp, Warning, TEXT("Pose %s"), *UEnum::GetValueAsString(CurrentWeaponType));
	
}