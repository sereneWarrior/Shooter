// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacter.h"

#include "PlayerAnimInstance.generated.h"

UCLASS()
class SHOOTER_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerAnimInstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		EWeaponType CurrentWeaponType = EWeaponType::Pistol;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		bool IsSwitchingWeapon;
		
protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	APlayerCharacter* Player;
};
