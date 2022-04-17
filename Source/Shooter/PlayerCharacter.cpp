// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "WeaponPickUp.h"
#include <Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#include "Shooter/MyHUD.h"



// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//TODO: REmove if not needed
	IsFirstSlotFilled = false;
	IsReloading = false;
	IsSwitchingWeapon = false;
	// TODO: It's not set yet but used in the Cross hair logic s. The default true should be temprary
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

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	PlayerInputComponent->BindAction("Weapon1", IE_Pressed, this, &APlayerCharacter::EquipW1);
	PlayerInputComponent->BindAction("Weapon2", IE_Pressed, this, &APlayerCharacter::EquipW2);
	PlayerInputComponent->BindAction("Weapon3", IE_Pressed, this, &APlayerCharacter::EquipW3);

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
	// Which apecific obect is weaponToSpawn:
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

//TODO: REfactor Equip methods
// TODO: Change to assertion. Maybe refactor logging.
void APlayerCharacter::EquipW1()
{
	if (!Weapons.IsValidIndex(0)) return;
	UE_LOG(LogTemp, Log, TEXT("Eqip %s"), *Weapons[0]->GetName());
	EqipWeapon(Weapons[0]);
}

void APlayerCharacter::EquipW2()
{
	if (!Weapons.IsValidIndex(1))  return;
	UE_LOG(LogTemp, Log, TEXT("Eqip %s"), *Weapons[1]->GetName());
	EqipWeapon(Weapons[1]);
}

void APlayerCharacter::EquipW3()
{
	if (!Weapons.IsValidIndex(2))  return;
	UE_LOG(LogTemp, Log, TEXT("Eqip %s"), *Weapons[2]->GetName());
	EqipWeapon(Weapons[2]);
}

void APlayerCharacter::EqipWeapon(AWeapon* weapon)
{
	UE_LOG(LogTemp, Log, TEXT("Eqips Weapon"));
	if (IsSwitchingWeapon) return;

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
	
	// Find the index of the current weapon in Weapon array
	

	// TODO: Would it make sense to fill array dynamically? If weapon is not in array add it?
	verify(Weapons.Find(weapon, Index));
	UE_LOG(LogTemp, Log, TEXT("Current weapon index: %d"), Index);
	checkf(CurrentWeapon != weapon, TEXT("Weapon is already current weapon"));

	//Hide current weapon mesh and show new weapon mesh
	//Handle from empty Loadout to Weapon:
	if (CurrentWeapon == nullptr) // TODO: Using Assertion?
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
