/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#include "JumpPadVolume.h"
#include "Defragr.h"
#include "Components/BrushComponent.h"

#include "DefragrPlayer.h"


// Sets default values
AJumpPadVolume::AJumpPadVolume(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Too lazy to make my own profile so we just use the TriggerVolume profile :3
	static FName CollisionProfileName(TEXT("Trigger"));
	GetBrushComponent()->SetCollisionProfileName(CollisionProfileName);

	OnActorBeginOverlap.AddDynamic(this, &AJumpPadVolume::OnActorEnteredVolume);

	bColored = true;
	BrushColor.R = 20;
	BrushColor.G = 255;
	BrushColor.B = 20;
	BrushColor.A = 255;
}

void AJumpPadVolume::OnActorEnteredVolume(AActor * Actor, AActor * Actor2)
{
	ADefragrPlayer* Player = Cast<ADefragrPlayer>(Actor);

	// If the actor that touched the pad is a player then launch the player
	if(Player && Enabled)
		Player->SetVelocity(JumpVelocity);
}

