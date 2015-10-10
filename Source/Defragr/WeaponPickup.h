/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#pragma once

#include "Weapon.h"

#include "Pickup.h"
#include "WeaponPickup.generated.h"

class ADefragrPlayer;


UCLASS(Blueprintable)
class DEFRAGR_API AWeaponPickup : public APickup
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = "Pickup", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeapon> Weapon;

public:
	void OnPlayerPickup(ADefragrPlayer* Player);

	UFUNCTION(BlueprintNativeEvent)
	void OnPickedUp();
};
