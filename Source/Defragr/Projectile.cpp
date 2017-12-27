/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#include "Projectile.h"
#include "Defragr.h"

#include "Weapon.h"
#include "DefragrPlayer.h"



AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	Weapon = Cast<AWeapon>(GetOwner());

	if(Weapon)
		Player = Weapon->Player;

	// Set the projectile's position and rotation based
	// on the player's camera.
	
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

