/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#include "TeleportVolume.h"
#include "Defragr.h"
#include "Components/BrushComponent.h"

#include "DefragrPlayer.h"
#include "DefragrPlayerMoveComponent.h"


// Sets default values
ATeleportVolume::ATeleportVolume(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Too lazy to make my own profile so we just use the TriggerVolume profile :3
	static FName CollisionProfileName(TEXT("Trigger"));
	GetBrushComponent()->SetCollisionProfileName(CollisionProfileName);

	OnActorBeginOverlap.AddDynamic(this, &ATeleportVolume::OnActorEnteredVolume);

	bColored = true;
	BrushColor.R = 255;
	BrushColor.G = 255;
	BrushColor.B = 100;
	BrushColor.A = 255;
}

void ATeleportVolume::OnActorEnteredVolume(AActor* Actor, AActor* Actor2)
{
	ADefragrPlayer* Player = Cast<ADefragrPlayer>(Actor);

	// Teleport the actor to the target if the target and
	// this volume is enabled and if the actor is of ADefragrPlayer type.
	if(Player && Enabled && Target && Target->Enabled)
	{
		Player->SetPosition(Target->GetTransform().GetLocation());

		if(Target->ResetViewingAngles)
			Player->SetRotation(Target->GetTransform().GetRotation());

		// Reset relative velocity
		if(Target->ResetVelocity)
			Player->SetRelativeVelocity(Target->ExitVelocity);

		Player->PlayTeleportSound();
	}
}