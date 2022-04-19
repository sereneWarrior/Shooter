// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "PlayerCharacter.h"
#include "Weapon.h"

#include "WeaponPickUp.generated.h"

UCLASS()
class SHOOTER_API AWeaponPickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponPickUp();

	//APlayerCharacter* Player;
	bool bPickup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWeapon> WeaponToSpawn;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class USoundWave* MySoundWave;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnIntateraction();

};
