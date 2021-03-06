// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Shooter/PlayerCharacter.h>
#include "Components/TextBlock.h"
#include <Runtime/UMG/Public/Components/CanvasPanel.h>
#include "Components/ProgressBar.h"

#include "WeaponInfoWidget.generated.h"

UCLASS()
class SHOOTER_API UWeaponInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UWeaponInfoWidget(const FObjectInitializer& ObjectInitializer);
	
	void SetCurrentAmmoValue();
	
protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* AmmoCount;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TotalAmmoCount;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Type;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* Panel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* Stamina;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

private:
	APlayerCharacter* Player;
};
