/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#pragma once

#include "GameFramework/Volume.h"
#include "JumpPadVolume.generated.h"

UCLASS()
class DEFRAGR_API AJumpPadVolume : public AVolume
{
	GENERATED_BODY()

	AJumpPadVolume(const class FObjectInitializer& ObjectIntializer);
	
public:	
	UPROPERTY(Category = "Jump Pad", EditAnywhere, BlueprintReadWrite)
	bool Enabled = true;

	/*  */
	UPROPERTY(Category = "Jump Pad", EditAnywhere, BlueprintReadWrite)
	FVector JumpVelocity;

public:
	UFUNCTION()
	void OnActorEnteredVolume(class AActor* Actor, class AActor* Actor2);
};
