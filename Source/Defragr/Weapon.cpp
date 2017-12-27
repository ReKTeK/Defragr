/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#include "Weapon.h"
#include "Defragr.h"

#include "Engine.h"

#include "Projectile.h"



AWeapon::AWeapon()
{
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));
	FirstPersonMesh->SetupAttachment(RootSceneComponent);

	ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Third Person Mesh"));
	ThirdPersonMesh->SetupAttachment(RootSceneComponent);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

FString AWeapon::GetWeaponName()
{
	return WeaponName;
}

void AWeapon::ExtraPickup()
{
	CurrentAmmo += ExtraPickupAmmo;
}

void AWeapon::Fire()
{
	// If there is a projectile assigned to the weapon then
	// ask it to spawn.
	if(Projectile)
		GetWorld()->SpawnActor<AProjectile>(Projectile);

	CurrentAmmo--;
}