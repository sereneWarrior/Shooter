// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon.h"
#include "TimerManager.h"


#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class ELoadout : uint8
{
	NoWeapon,
	HasGun
};

UCLASS()
class SHOOTER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);


private:
	FTimerHandle UnusedHandle;

	AWeapon* HandleWeaponSpawning(TSubclassOf<class AWeapon> weapon, USkeletalMeshComponent* armsMesh);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Interact();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		ELoadout Loadout = ELoadout::NoWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<AWeapon> PistolClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<AWeapon> ShotgunClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<AWeapon> AutoRifleClass;

	bool IsFirstSlotFilled;
	bool IsSwitchingWeapon;
	bool IsReloading;
	// TODO: Setting ot yet implemented. Comes (probably at a later point in the tutorial.
	bool IsAiming;
	class AWeapon* CurrentWeapon;
	
	TArray<AWeapon*> Weapons;
	
	AWeapon* SpawnWeapon(TSubclassOf<class AWeapon> weaponToSpawn);
	void EqipWeapon(AWeapon* weapon);
	void SwitchWeaponMesh(AWeapon* weapon);
	// TOD: Rename!
	void EquipW1();
	void EquipW2();
	void EquipW3();
};
