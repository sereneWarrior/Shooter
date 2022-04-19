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
// Here thethe vale the Action key should pass in when the Input is pressed is defined. 
// It uses an int32 as an Index to the Weapon Slot.
// https://amiransari09.medium.com/unreal-engine-advance-key-binding-653266e1db22
DECLARE_DELEGATE_OneParam(FInputSwitchWeaponDelegate, int);

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
	void HandleWeaponSlotInput(int weaponKey);

public:	
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

	bool IsSwitchingWeapon;
	bool IsReloading;
	// TODO: Setting ot yet implemented. Comes (probably at a later point in the tutorial.
	bool IsAiming;
	class AWeapon* CurrentWeapon;
	
	TArray<AWeapon*> Weapons;
	
	AWeapon* SpawnWeapon(TSubclassOf<class AWeapon> weaponToSpawn);
	void EqipWeapon(AWeapon* weapon);
	void SwitchWeaponMesh(AWeapon* weapon);
};
