// Fill out your copyright notice in the Description page of Project Settings.


#include "CrossHair.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"


UCrossHair::UCrossHair(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InterpSpeed = 30.0f;
	FallingTarget = -60.0f;
	IsVisible = false;
}

void UCrossHair::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);

	CrossHairUpdate();
}

// Show cross hair and change settings
void UCrossHair::OnWeaponChanged(float spread, float speed, bool isVisible)
{
	BulletSpreadHUD = (spread / 0.5) * -1;
	IsVisible = isVisible;
	SetPanelVisibility();
	//CrossHairUpdate();
}


void UCrossHair::MoveCrossHairs(float target)
{
	Target = target + BulletSpreadHUD;
	auto deltaTime = GetWorld()->GetDeltaSeconds();

	float InterpolationTop = FMath::FInterpTo(TopCrossHair->RenderTransform.Translation.Y, Target, deltaTime, InterpSpeed);
	FVector2D TranslationTop(TopCrossHair->RenderTransform.Translation.X, InterpolationTop);
	TopCrossHair->SetRenderTranslation(TranslationTop);

	float InterpolationBottom = FMath::FInterpTo(BottomCrossHair->RenderTransform.Translation.Y, Target * -1, deltaTime, InterpSpeed);
	FVector2D TranslationBottom(BottomCrossHair->RenderTransform.Translation.X, InterpolationBottom);
	BottomCrossHair->SetRenderTranslation(TranslationBottom);

	float InterpolationRight = FMath::FInterpTo(RightCrossHair->RenderTransform.Translation.X, Target * -1, deltaTime, InterpSpeed);
	FVector2D TranslationRight(InterpolationRight, RightCrossHair->RenderTransform.Translation.Y);
	RightCrossHair->SetRenderTranslation(TranslationRight);

	float InterpolationLeft = FMath::FInterpTo(LeftCrossHair->RenderTransform.Translation.X, Target, deltaTime, InterpSpeed);
	FVector2D TranslationLeft(InterpolationLeft, LeftCrossHair->RenderTransform.Translation.Y);
	LeftCrossHair->SetRenderTranslation(TranslationLeft);
}

// CrossHairUpdate handles the change of the crosshair appearance depending on the weapon held bay the player.
void UCrossHair::CrossHairUpdate()
{
	auto isPanelVisible = SetPanelVisibility();
	if (!isPanelVisible) return;

	//BulletSpreadHUD = (Player->CurrentWeapon->BulletSpread / 0.5) * -1;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &UCrossHair::TranslateCrossHair, 0.01f, true);
}

void UCrossHair::TranslateCrossHair()
{
	// If character is falling
	/*if (Player->GetCharacterMovement()->IsFalling())
	{
		// TODO: Is Falling is not yet used by PlayerCharacter.
		MoveCrossHairs(FallingTarget);
	}*/
	if (Speed > 1.0f)
	{
		MoveCrossHairs((Speed * -1) / 10.0f);
	}
	// If character is standing
	MoveCrossHairs(0);
}

bool UCrossHair::SetPanelVisibility()
{
	if (!IsVisible)
	{
		Panel->SetVisibility(ESlateVisibility::Collapsed);
		return false;
	}

	Panel->SetVisibility(ESlateVisibility::Visible);
	return true;
}

void UCrossHair::SetPlayerSpeed(float speed)
{
	Speed = speed;
	CrossHairUpdate();

}


