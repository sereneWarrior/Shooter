#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Pistol,
	AutoRifle, 
	Shotgun 
};

UCLASS()
class SHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CurrentTotalAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CurrentAmmoCount;


	float BulletSpread;
};
