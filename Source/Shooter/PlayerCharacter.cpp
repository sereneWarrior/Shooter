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
		UE_LOG(LogTemp, Warning, TEXT("No hit"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("hit"));
	//DrawDebugLine(GetWorld(), Start, End, FColor::Orange, true, 10.0f);
	if (AWeaponPickUp* hitActor = Cast<AWeaponPickUp>(HitDetails.GetActor()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *hitActor->GetName());
		hitActor->OnIntateraction();
	}
	
}

AWeapon* APlayerCharacter::SpawnWeapon(TSubclassOf<class AWeapon> weaponToSpawn)
{
	//TODO Refactor
	auto ArmsMesh = Cast<USkeletalMeshComponent>(this->GetDefaultSubobjectByName(TEXT("PlayerArms")));
	if (weaponToSpawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Weapon to Spawn."));
		return nullptr;
	}
	if (ArmsMesh == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Error getting mesh component."));
		return nullptr;
	}

	
	//IsFirstSlotFilled = true;

	//Spawns weapon not subclass
	if (weaponToSpawn->GetName() == PistolClass->GetName())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Pistol"));
		auto weapon = GetWorld()->SpawnActor<AWeapon>(PistolClass);
		Weapons.Emplace(weapon);
		weapon->AttachToComponent(ArmsMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "PistolSocket");
		
		EqipWeapon(weapon);
		return weapon;
	}
	if (weaponToSpawn->GetName() == ShotgunClass->GetName())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn ShotGun"));
		auto weapon = GetWorld()->SpawnActor<AWeapon>(ShotgunClass);
		Weapons.Emplace(weapon);
		weapon->AttachToComponent(ArmsMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "ShotgunSocket");
		
		EqipWeapon(weapon);
		return weapon;
	}
	if (weaponToSpawn->GetName() == AutoRifleClass->GetName())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn AutoRifle"));
		auto weapon = GetWorld()->SpawnActor<AWeapon>(AutoRifleClass);
		Weapons.Add(weapon);
		weapon->AttachToComponent(ArmsMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "AutoRifleSocket");
		
		EqipWeapon(weapon);
		return weapon;
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("Socket %s"), *weapon->SocketName.ToString());
	UE_LOG(LogTemp, Error, TEXT("No weapon matched"));
	return nullptr;
}

//TODO: REfactor Equip methods
void APlayerCharacter::EquipW1()
{
	if (!Weapons.IsValidIndex(0)) return;
	UE_LOG(LogTemp, Warning, TEXT("Eqip %s"), *Weapons[0]->GetName());
	EqipWeapon(Weapons[0]);
}

void APlayerCharacter::EquipW2()
{
	if (!Weapons.IsValidIndex(1))  return;
	UE_LOG(LogTemp, Warning, TEXT("Eqip %s"), *Weapons[1]->GetName());
	EqipWeapon(Weapons[1]);
}

void APlayerCharacter::EquipW3()
{
	if (!Weapons.IsValidIndex(2))  return;
	UE_LOG(LogTemp, Warning, TEXT("Eqip %s"), *Weapons[2]->GetName());
	EqipWeapon(Weapons[2]);
}

void APlayerCharacter::EqipWeapon(AWeapon* weapon)
{
	UE_LOG(LogTemp, Warning, TEXT("Eqips Weapon"));
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
	AMyHUD* MyHud = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	
	//Hide current weapon mesh and show new weapon mesh

	// TODO: Implement go to next weaopn slot
	// TODO: REfactor!!!!!!
	
	int32 Index;
	Weapons.Find(weapon, Index);
	UE_LOG(LogTemp, Warning, TEXT("index %d"),Index);
	if (Index == INDEX_NONE)
	{
		//TODO: Check this. It is trigered when first weapon is taken.
		//UE_LOG(LogTemp, Warning,TEXT("Weapon not in inventory"));
		return;
	}

	if (CurrentWeapon == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("First weapon"));
		Loadout = ELoadout::HasGun;
		CurrentWeapon = weapon;
		if (MyHud)
		{
			MyHud->UpdateCrossHairWidget();
		}
		return;
	}

	if (CurrentWeapon == weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon is already current weapon"));
		return;
	}
	CurrentWeapon->SetActorHiddenInGame(true);
		
	UE_LOG(LogTemp, Warning, TEXT("Switching wepon from %s to %s"), *CurrentWeapon->GetName(), *Weapons[Index]->GetName());
	
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
