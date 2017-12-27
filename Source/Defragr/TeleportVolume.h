/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#pragma once

#include "GameFramework/Volume.h"
#include "TeleportTarget.h"
#include "TeleportVolume.generated.h"

UCLASS(hidecategories=(Collision,Actor))
class DEFRAGR_API ATeleportVolume : public AVolume
{
	GENERATED_BODY()

	ATeleportVolume(const class FObjectInitializer& ObjectIntializer);

public:
	/* If unticked this teleport volume will not cause the player to teleport */
	UPROPERTY(Category = "Teleport Volume", EditAnywhere, BlueprintReadWrite)
	bool Enabled = true;

	/* Target teleport position */
	UPROPERTY(Category = "Teleport Volume", EditAnywhere, BlueprintReadWrite)
	ATeleportTarget* Target = NULL;

public:
	UFUNCTION()
	virtual void OnActorEnteredVolume(class AActor* Actor, class AActor* Actor2);
};
