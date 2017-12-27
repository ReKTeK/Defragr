/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#pragma once

#include "GameFramework/Volume.h"
#include "Checkpoint.generated.h"

UENUM(BlueprintType)
enum class ECheckpointType : uint8
{
	Start,
	Split,
	End
};

UCLASS()
class DEFRAGR_API ACheckpoint : public AVolume
{
	GENERATED_BODY()
	
	ACheckpoint(const class FObjectInitializer& ObjectIntializer);

public:
	UPROPERTY(Category = "Checkpoint", EditAnywhere, BlueprintReadWrite)
	ECheckpointType CheckpointType = ECheckpointType::Split;

	/* The next checkpoint (usually split or end types) */
	UPROPERTY(Category = "Checkpoint", EditAnywhere, BlueprintReadWrite)
	ACheckpoint* NextCheckpoint;

public:
	UFUNCTION()
	virtual void OnActorEnteredVolume(class AActor* Actor, class AActor* Actor2);

	UFUNCTION()
	virtual void OnActorExitedVolume(class AActor* Actor, class AActor* Actor2);
};
