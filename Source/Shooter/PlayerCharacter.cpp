// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "WeaponPickUp.h"
#include <Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#include "Shooter/MyHUD.h"
#include <Shooter/PlayerCharacter.h>



// Sets default values
APlayerCharacter::APlayerCharacter()
{
	IsReloading = false;
	IsSwitchingWeapon = false;
	// TODO: It's not being set yet but used in the Cross hair logic.
	IsAiming = true;
	CurrentWeapon = nullptr;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

AWeapon* APlayerCharacter::HandleWeaponSpawning(TSubclassOf<class AWeapon> weaponClass, USkeletalMeshComponent* armsMesh)
{
	auto weapon = GetWorld()->SpawnActor<AWeapon>(weaponClass);
	UE_LOG(LogTemp, Log, TEXT("Spawning %s"), *UEnum::GetDisplayValueAsText(weapon->WeaponType).ToString());
	Weapons.Emplace(weapon);
	weapon->AttachToComponent(armsMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "ShotgunSocket");

	EqipWeapon(weapon);
	return weapon;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction<FInputSwitchWeaponDelegate>("Weapon1", IE_Pressed, this, &APlayerCharacter::HandleWeaponSlotInput, 0);
	PlayerInputComponent->BindAction<FInputSwitchWeaponDelegate>("Weapon2", IE_Pressed, this, &APlayerCharacter::HandleWeaponSlotInput, 1);
	PlayerInputComponent->BindAction<FInputSwitchWeaponDelegate>("Weapon3", IE_Pressed, this, &APlayerCharacter::HandleWeaponSlotInput, 2);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
}

void APlayerCharacter::Interact()
{
	//TODO: Camera is attached to player, use child component
	auto camera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	auto cameraLocation = camera->GetCameraLocation();
	auto cameraDirection = camera->GetCameraRotation();
	FHitResult HitDetails = FHitResult(ForceInit);
	FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, NULL);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;

	FVector Start = cameraLocation;
	FVector End = Start + (cameraDirection.Vector() * 2000.0f); // 200.9f is linetracedistance

	bool bIsHit = GetWorld()->LineTraceSingleByChannel(HitDetails, 
		Start,
		End,
		ECC_Visibility,
		TraceParams);
	
	if (!bIsHit)
	{
		UE_LOG(LogTemp, Log, TEXT("No hit"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("hit"));
	
	// TODO: Change to assertion. Maybe refactor logging.
	if (AWeaponPickUp* hitActor = Cast<AWeaponPickUp>(HitDetails.GetActor()))
	{
		UE_LOG(LogTemp, Log, TEXT("Hit %s"), *hitActor->GetName());
		hitActor->OnIntateraction();
	}
}

AWeapon* APlayerCharacter::SpawnWeapon(TSubclassOf<class AWeapon> weaponToSpawn)
{
	check(weaponToSpawn);

	USkeletalMeshComponent* ArmsMesh;
	verify((ArmsMesh = Cast<USkeletalMeshComponent>(this->GetDefaultSubobjectByName(TEXT("PlayerArms")))) != nullptr);
	
	//IsFirstSlotFilled = true;

	// TODO: Isn't it possible to have something like weaponToSpawn = EWeapon::Pistol?
	
	auto weaponName = weaponToSpawn->GetName();
	// Which specific child class of AWeapon is weaponToSpawn of:
	if (weaponName == PistolClass->GetName())
	{
		return HandleWeaponSpawning(PistolClass, ArmsMesh);
	}
	if (weaponToSpawn->GetName() == ShotgunClass->GetName())
	{
		return HandleWeaponSpawning(ShotgunClass, ArmsMesh);
	}
	if (weaponToSpawn->GetName() == AutoRifleClass->GetName())
	{
		return HandleWeaponSpawning(AutoRifleClass, ArmsMesh);
	}
	
	UE_LOG(LogTemp, Error, TEXT("No weapon matched"));
	checkNoEntry();
	return nullptr;
}

void APlayerCharacter::HandleWeaponSlotInput(int slotNumber)
{
	if (!Weapons.IsValidIndex(slotNumber))  return;
	if (CurrentWeapon == Weapons[slotNumber]) return;
	// TODO: Create TMap to store weapons and associated weapon slot.
	EqipWeapon(Weapons[slotNumber]);
}

void APlayerCharacter::EqipWeapon(AWeapon* weapon)
{
	if (IsSwitchingWeapon) return;
	UE_LOG(LogTemp, Log, TEXT("Eqips %s"), *UEnum::GetDisplayValueAsText(weapon->WeaponType).ToString());
	
	if (IsReloading) IsReloading = false;

	IsSwitchingWeapon = true;
	SwitchWeaponMesh(weapon);
	
	GetWorldTimerManager().SetTimer(UnusedHandle, [this]() {
		IsSwitchingWeapon = false;
		}, 0.33f, 1);
}

void APlayerCharacter::SwitchWeaponMesh(AWeapon* weapon)
{
	check(weapon != nullptr);

	AMyHUD* MyHud; 
	int32 Index;
	verify((MyHud = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())) != nullptr);
	
	// Find the index of the current weapon in Weapon array.
	// TODO: Would it make sense to fill array dynamically? If weapon is not in array add it?
	verify(Weapons.Find(weapon, Index)); // Program stops if weapon not in array!
	UE_LOG(LogTemp, Log, TEXT("Current weapon index: %d"), Index);

	// It should not happen that a wepon that is already active ist equiped. But if it happens log it.
	if (CurrentWeapon == weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried to set a weapon thats already active"));
		return;
	}

	//Hide current weapon mesh and show new weapon mesh
	//Handle from empty Loadout to Weapon:
	if (CurrentWeapon == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("First weapon set"));
	}
	else
	{
		// If there was a weapon held hide it's mesh.
		CurrentWeapon->SetActorHiddenInGame(true);
		
		UE_LOG(LogTemp, Log, TEXT("Switching wepon from %s to %s"),
			*UEnum::GetDisplayValueAsText(CurrentWeapon->WeaponType).ToString(),
			*UEnum::GetDisplayValueAsText(Weapons[Index]->WeaponType).ToString());
	}
	
	// Show the weapon that was equiped.
	Weapons[Index]->SetActorHiddenInGame(false);
	Loadout = ELoadout::HasGun;
	CurrentWeapon = weapon;
	if (MyHud)
	{
		MyHud->UpdateCrossHairWidget();
	}
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector() * Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector() * Value);
	}
}
