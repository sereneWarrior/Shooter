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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		float Speed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		EWeaponType CurrentWeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		bool IsSwitchingWeapon = false;
		
protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
