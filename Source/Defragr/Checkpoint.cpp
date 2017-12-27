/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */


#include "Checkpoint.h"
#include "Defragr.h"
#include "Components/BrushComponent.h"

#include "DefragrPlayer.h"


ACheckpoint::ACheckpoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Too lazy to make my own profile so we just use the TriggerVolume profile :3
	static FName CollisionProfileName(TEXT("Trigger"));
	GetBrushComponent()->SetCollisionProfileName(CollisionProfileName);

	// Set up events
	OnActorBeginOverlap.AddDynamic(this, &ACheckpoint::OnActorEnteredVolume);
	OnActorEndOverlap.AddDynamic(this, &ACheckpoint::OnActorExitedVolume);

	// Set the color based on the default checkpoint type,
	// we change the color later on.
	bColored = true;
	BrushColor.R = 255;
	BrushColor.G = 100;
	BrushColor.B = 255;
	BrushColor.A = 255;
}

void ACheckpoint::OnActorEnteredVolume(AActor* Actor, AActor* Actor2)
{
	if(CheckpointType == ECheckpointType::End)
	{
		ADefragrPlayer* Player = Cast<ADefragrPlayer>(Actor);

		if(Player)
			Player->StopRaceTimer();
	}
}

void ACheckpoint::OnActorExitedVolume(AActor* Actor, AActor* Actor2)
{
	// If the checkpoint is a starting checkpoint then start the
	// race timer for the player.
	if(CheckpointType == ECheckpointType::Start)
	{
		ADefragrPlayer* Player = Cast<ADefragrPlayer>(Actor);

		if(Player && Player->RaceTime == 0.f)
			Player->StartRaceTimer();
	}
}