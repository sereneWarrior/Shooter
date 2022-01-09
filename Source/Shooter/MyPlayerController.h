// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerPawn.h"

#include "MyPlayerController.generated.h"


UCLASS()
class SHOOTER_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()


protected:

	void SetupInputComponent() override;


public:

	// Mouse movements
};
