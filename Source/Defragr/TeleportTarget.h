/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "TeleportTarget.generated.h"

UCLASS(hidecategories=(Input,Rendering,Actor))
class DEFRAGR_API ATeleportTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	ATeleportTarget(const class FObjectInitializer& ObjectIntializer);

public:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UArrowComponent* ArrowComponent;

	UPROPERTY()
	UBillboardComponent* SpriteComponent;

	UPROPERTY()
	UTexture2D* SpriteTexture;

	UPROPERTY()
	UBoxComponent* PlayerExtents;
#endif

public:
	UPROPERTY(Category = "Teleport", EditAnywhere, BlueprintReadWrite)
	bool Enabled = true;

	/* Sets the player's velocity to "Exit Velocity" after teleporting if ticked */
	UPROPERTY(Category = "Teleport", EditAnywhere, BlueprintReadWrite)
	bool ResetVelocity = true;

	/* Velocity relative to the player */
	UPROPERTY(Category = "Teleport", EditAnywhere, BlueprintReadWrite)
	FVector ExitVelocity = FVector(320.f, 0.f, 0.f);

	/* Resets the player's viewing angle to be facing the same way as the teleport */
	UPROPERTY(Category = "Teleport", EditAnywhere, BlueprintReadWrite)
	bool ResetViewingAngles = true;
};
