// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Runtime/UMG/Public/Components/CanvasPanel.h>
#include "Components/SizeBox.h"
#include "TimerManager.h"

#include "CrossHair.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerWalking, float, PlayerVelocity);

/**
 * Base class for UMG Widgets that belong to a single Actor in the world via a WidgetComponent.
 */
UCLASS(Abstract)
class SHOOTER_API UCrossHair : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
		void SetPlayerSpeed(float speed);

	UPROPERTY(BlueprintAssignable, Category = "UI")
		FOnPlayerWalking OnPlayerWalking;

	UCrossHair(const FObjectInitializer& ObjectInitializer);

	void CrossHairUpdate();
	void OnWeaponChanged(float spread, float speed, bool isVisible);
	float Speed;

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* Panel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class USizeBox* TopCrossHair;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* TopFillImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class USizeBox* BottomCrossHair;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* BottomFillImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class USizeBox* LeftCrossHair;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* LeftFillImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class USizeBox* RightCrossHair;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* RightFillImage;

	UPROPERTY(EditAnywhere)
	float InterpSpeed;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

private:

	float BulletSpreadHUD;
	float Target;
	float FallingTarget;
	bool IsVisible;

	FTimerHandle UnusedHandle;

	void TranslateCrossHair();
	bool SetPanelVisibility();
	void MoveCrossHairs(float target);

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn = true))
		AActor* AttachedActor;
};
