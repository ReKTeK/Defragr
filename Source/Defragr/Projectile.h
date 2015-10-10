/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#pragma once

#include "Components/StaticMeshComponent.h"

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class AWeapon;
class ADefragrPlayer;


UCLASS()
class DEFRAGR_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	AWeapon* Weapon;
	ADefragrPlayer* Player;

public:
	USkeletalMeshComponent* Mesh;

	/* If true then the projectile is an insta hit */
	UPROPERTY(Category = "Projectile", EditAnywhere, BlueprintReadWrite)
	bool NoFlight = false;

	/* Speed of the projectile, only works if No Flight is false */
	UPROPERTY(Category = "Projectile", EditAnywhere, BlueprintReadWrite)
	float FlightSpeed = 1.0f;

	/* Damage of the projectile upon hitting something */
	UPROPERTY(Category = "Projectile", EditAnywhere, BlueprintReadWrite)
	float Damage = 0.0f;

	/* Splash damage maximum falloff */
	UPROPERTY(Category = "Projectile", EditAnywhere, BlueprintReadWrite)
	float DamageMaxRange = 0.0f;

	/* Splash damage minimum falloff */
	UPROPERTY(Category = "Projectile", EditAnywhere, BlueprintReadWrite)
	float DamageMinRange = 0.0f;

	/* Velocity of the entity that's hit */
	float KnockbackVelocity = 0.0f;

public:
	void Spawn();
};
