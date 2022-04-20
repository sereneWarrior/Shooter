// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BulletSpread = 50.0f;
	CurrentTotalAmmo = 30;
	CurrentAmmoCount = 5;
	AmmoType.FromString(FString("(AUTO)"));
}
