/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#pragma once

#include "DefragrPlayer.h"

#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class AProjectile;

UENUM(BlueprintType)
enum class EWeapons : uint8
{
	Default          UMETA(DisplayName = "Default"),
	RocketLauncher   UMETA(DisplayName = "Rocket Launcher")
};


UCLASS(Blueprintable)
class DEFRAGR_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void BeginPlay() override;

public:
	/* The player this weapon belongs to */
	ADefragrPlayer* Player;

	USceneComponent* RootSceneComponent;

	UPROPERTY(Category = "Weapon", VisibleDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(Category = "Weapon",  VisibleDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* ThirdPersonMesh;

	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AProjectile> Projectile;

public:
	/* The amount of ammo the weapon currently has */
	UPROPERTY(Category = "Ammo", EditAnywhere, BlueprintReadWrite)
	int32 CurrentAmmo = 0;

	/* The max amount of ammo the weapon can carry */
	UPROPERTY(Category = "Ammo", EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmo = 99;

	/* How much one shot costs */
	UPROPERTY(Category = "Ammo", EditAnywhere, BlueprintReadWrite)
	int32 AmmoCost = 1;

	/* The amount of ammo the weapon has when first given to the player */
	UPROPERTY(Category = "Ammo", EditAnywhere, BlueprintReadWrite)
	int32 PickupAmmo = 10;

	/* The amount of ammo that's given to the player if he already has this weapon */
	UPROPERTY(Category = "Ammo", EditAnywhere, BlueprintReadWrite)
	int32 ExtraPickupAmmo = 5;

	/* If true, firing the weapon doesn't cost any ammo */
	UPROPERTY(Category = "Ammo", EditAnywhere, BlueprintReadWrite)
	bool InfiniteAmmo = false;

public:
	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadWrite)
	FString WeaponName = "Default Weapon";

	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadWrite)
	uint8 SlotID = 255;

	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadWrite)
	EWeapons WeaponType = EWeapons::Default;

	/* In degrees */
	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadWrite)
	float Spread = 0.0f;

	/* Units before the weapon is ineffective */
	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadWrite)
	float Range = 1000.0f;

public:
	UFUNCTION(Category = "Weapon", BlueprintCallable)
	FString GetWeaponName();

public:
	UFUNCTION(Category = "Ammo", BlueprintCallable)
	void ExtraPickup();

	UFUNCTION(Category = "Weapon", BlueprintCallable)
	void Fire();
};
