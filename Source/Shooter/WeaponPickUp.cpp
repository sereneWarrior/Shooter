// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickUp.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
AWeaponPickUp::AWeaponPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bPickup = false;
}

// Called when the game starts or when spawned
void AWeaponPickUp::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AWeaponPickUp::Tick(float DeltaTime)
{

}

void AWeaponPickUp::OnIntateraction()
{
	if (Player == nullptr) return;

	//TODO: use event from blueprint
	Player->SpawnWeapon(WeaponToSpawn);
	if (!bPickup)
	{
		bPickup = true;

		UGameplayStatics::PlaySound2D(this, MySoundWave);
		
	}
	bPickup = false;

	Destroy();
}




