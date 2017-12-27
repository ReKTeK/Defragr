/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#include "DefragrPlayerCollisionComponent.h"
#include "Defragr.h"
#include "DefragrPlayer.h"
#include "DefragrPlayerMoveComponent.h"

#include "Engine.h"


// Sets default values for this component's properties
UDefragrPlayerCollisionComponent::UDefragrPlayerCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UDefragrPlayerCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UDefragrPlayerCollisionComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Delta = DeltaTime;
}

void UDefragrPlayerCollisionComponent::TraceGround()
{
	bool Hit = Trace(GroundTrace, MovementComponent->GetOrigin(), MovementComponent->GetOrigin() - FVector(0.f, 0.f, GroundTraceDistance));

	// No hits then the player cannot ground move
	if(GroundTrace.Time == 1.0f)
	{
		IsGrounded    = false;
		CanGroundMove = false;

		return;
	}

	// Check if getting thrown off the ground
	if(MovementComponent->PlayerVelocity.Z > 0 && FVector::DotProduct(MovementComponent->PlayerVelocity, GroundTrace.ImpactNormal) > 10)
	{
		IsGrounded    = false;
		CanGroundMove = false;

		return;
	}

	// Slopes that are too steep will not be considered walkable
	if(GroundTrace.ImpactNormal.Z < MinWalkNormal)
	{
		IsGrounded    = true;
		CanGroundMove = false;

		return;
	}

	IsGrounded    = true;
	CanGroundMove = true;

	// If the player just hit the ground

}

bool UDefragrPlayerCollisionComponent::SlideMove(bool Gravity)
{
	int32 NumPlanes;
	int32 NumBumps;
	FVector Planes[5];
	FVector EndVelocity;

	NumPlanes = 0;
	NumBumps = 4;

	if(Gravity)
	{
		EndVelocity = MovementComponent->PlayerVelocity;
		EndVelocity.Z -= MovementComponent->Gravity * Delta;
		MovementComponent->PlayerVelocity.Z = (MovementComponent->PlayerVelocity.Z + EndVelocity.Z) * 0.5f;

		if(IsGrounded)
		{
			MovementComponent->ClipVelocity(MovementComponent->PlayerVelocity, GroundTrace.ImpactNormal, MovementComponent->PlayerVelocity, Overclip);
		}
	}

	// This is used later to resolve collisions
	float TimeLeft = Delta;

	// Never turn against the ground plane
	if(IsGrounded)
	{
		NumPlanes = 1;
		Planes[0] = GroundTrace.ImpactNormal;
	}

	// Never turn against the original velocity
	Planes[NumPlanes] = MovementComponent->PlayerVelocity;
	Planes[NumPlanes].Normalize();
	NumPlanes++;

	int32 BumpCount = 0;
	for(; BumpCount < NumBumps; BumpCount++)
	{
		// Calculate the position we are trying to move to
		FVector End = FVector::ZeroVector;
		VectorMA(MovementComponent->GetOrigin(), TimeLeft, MovementComponent->PlayerVelocity, End);

		// See if we can get there
		FHitResult PartialTrace;
		Trace(PartialTrace, MovementComponent->GetOrigin(), End);

		// If there was no hit then move the player the entire way and stop
		// this iteration, however if there was a hit then move the player
		// along the trace until the hit.
		MovementComponent->SetOrigin(TraceEnd(PartialTrace));
		if(!PartialTrace.bBlockingHit)
			break;

		// TODO: Record the entity that was collided with

		TimeLeft -= TimeLeft * PartialTrace.Time;

		if(NumPlanes >= MaxClipPlanes)
		{
			MovementComponent->PlayerVelocity = FVector::ZeroVector;
			return true;
		}

		// If this is the same plane we hit before, nudge the velocity
		// along it, which fixes some epsilon issues with non-axial planes.
		int32 i;
		for(i = 0; i < NumPlanes; i++)
		{
			float DotProd = FVector::DotProduct(PartialTrace.ImpactNormal, Planes[i]);
			if(DotProd > 0.99f)
			{
				MovementComponent->PlayerVelocity += PartialTrace.ImpactNormal;
				break;
			}
		}
		if(i < NumPlanes)
			continue;
		Planes[NumPlanes] = PartialTrace.ImpactNormal;
		NumPlanes++;

		// We modify the velocity of the player so it parallels all the clip planes
		i = 0;
		for(i = 0; i < NumPlanes; i++)
		{
			FVector ClipVelocity;
			FVector EndClipVelocity;

			// Find a plane that the player enters
			float Into = FVector::DotProduct(MovementComponent->PlayerVelocity, Planes[i]);
			// The move doesn't interact with the plane
			if(Into >= 0.1f)
				continue;

			// TODO: Impact magnitude
			// FROM: bg_slidemove.c
			// 
			// if ( -into > pml.impactSpeed ) {
			// 	pml.impactSpeed = -into;
			// }

			// Slide along the plane
			MovementComponent->ClipVelocity(MovementComponent->PlayerVelocity, Planes[i], ClipVelocity, Overclip);
			MovementComponent->ClipVelocity(EndVelocity, Planes[i], EndClipVelocity, Overclip);

			// Check for a second plane that the new move enters
			for(int32 j = 0; j < NumPlanes; j++)
			{
				if(j == i)
					continue;
				// The move doesn't interact with the plane
				if(FVector::DotProduct(ClipVelocity, Planes[j]) >= 0.1f)
					continue;

				// Slide along the plane
				MovementComponent->ClipVelocity(ClipVelocity, Planes[j], ClipVelocity, Overclip);
				MovementComponent->ClipVelocity(EndClipVelocity, Planes[j], EndClipVelocity, Overclip);

				// If it goes back into the first clip plane
				if(FVector::DotProduct(ClipVelocity, Planes[j]) >= 0.1f)
					continue;

				// Slide the original velocity along the intersection
				// of the two planes
				FVector Dir;
				float D;

				Dir = FVector::CrossProduct(Planes[i], Planes[j]);
				Dir.Normalize();
				D = FVector::DotProduct(Dir, MovementComponent->PlayerVelocity);
				ClipVelocity = Dir * D;

				Dir = FVector::CrossProduct(Planes[i], Planes[j]);
				Dir.Normalize();
				D = FVector::DotProduct(Dir, EndVelocity);
				EndClipVelocity = Dir * D;

				// Check for third plane intersection
				for(int32 k = 0; k < NumPlanes; k++)
				{
					if (k == i || k == j)
						continue;
					// The move doesn't interact with the plane
					if (FVector::DotProduct(ClipVelocity, Planes[k]) >= 0.1f)
						continue;

					// Stop dead
					MovementComponent->PlayerVelocity = FVector::ZeroVector;
					GEngine->AddOnScreenDebugMessage(20, 0.01f, FColor::Red, TEXT("Player is stuck"));

					return true;
				}
			}

			// Fixed all plane interactions, continue trying another
			// move.
			MovementComponent->PlayerVelocity = ClipVelocity;
			EndVelocity = EndClipVelocity;

			break;
		}
	}

	if(Gravity)
		MovementComponent->PlayerVelocity = EndVelocity;

	return (BumpCount != 0);
}

void UDefragrPlayerCollisionComponent::StepSlideMove(bool Gravity)
{
	FVector StartO, StartV;
	FVector DownO, DownV;
	FVector Down, Up;
	FHitResult StepTrace;
	float StepSize;

	StartO = MovementComponent->GetOrigin();
	StartV = MovementComponent->PlayerVelocity;

	// Got where we wanted on the first go
	if(SlideMove(Gravity) == 0)
		return;
	
	Down = StartO;
	Down.Z -= MaxStepSize;
	Trace(StepTrace, StartO, Down);
	Up = FVector::ZeroVector;
	Up.Z = 1;
	// Never step up if the player still has up velocity
	if (MovementComponent->PlayerVelocity.Z > 0.f &&
		(StepTrace.Time == 1.0f || FVector::DotProduct(StepTrace.ImpactNormal, Up) < 0.7f))
		return;

	DownO = MovementComponent->GetOrigin();
	DownV = MovementComponent->PlayerVelocity;

	Up = StartO;
	Up.Z += MaxStepSize;

	Trace(StepTrace, StartO, Up);
	if(StepTrace.bStartPenetrating)
		return; // Can't step up

	StepSize = TraceEnd(StepTrace).Z - StartO.Z;
	MovementComponent->SetOrigin(TraceEnd(StepTrace));
	MovementComponent->PlayerVelocity = StartV;

	SlideMove(Gravity);

	// Push down the final amount
	Down = MovementComponent->GetOrigin();
	Down.Z -= StepSize;
	Trace(StepTrace, MovementComponent->GetOrigin(), Down);
	MovementComponent->SetOrigin(TraceEnd(StepTrace));

	if(StepTrace.Time < 1.0f)
	{
		MovementComponent->ClipVelocity(MovementComponent->PlayerVelocity, StepTrace.ImpactNormal, MovementComponent->PlayerVelocity, Overclip);
	}
}

bool UDefragrPlayerCollisionComponent::Trace(FHitResult& Result, FVector Start, FVector End)
{
	// Get the player's collider extents (note that in this case
	// the collider is required to be a box).
	FCollisionShape CollisionShape = Player->Collider->GetCollisionShape();

	FCollisionQueryParams QueryParams(TEXT(""), false);
	FCollisionObjectQueryParams ObjectQueryParams(ECC_WorldStatic);

	bool Hit = GetWorld()->SweepSingleByObjectType(Result, Start, End, FQuat::Identity, ObjectQueryParams, CollisionShape, QueryParams);
	
	return Hit;
}

FVector UDefragrPlayerCollisionComponent::TraceEnd(FHitResult& T)
{
	// If the trace started in a collider then we calculate the escaped
	// vector and return it.
	if(T.bStartPenetrating)
	{
		FVector Loc = T.TraceStart + T.ImpactNormal * (T.PenetrationDepth) + (T.Normal * Underclip);
		GEngine->AddOnScreenDebugMessage(50, 0.01f, FColor::Yellow, FString::Printf(TEXT("TraceEnd: Loc [X: %.6f, Y: %.6f, Z: %.6f]"), T.Normal.X, T.Normal.Y, T.Normal.Z));
		GEngine->AddOnScreenDebugMessage(51, 0.01f, FColor::Yellow, FString::Printf(TEXT("TraceEnd: PenDepth [X: %.8f]"), T.PenetrationDepth));

		return Loc;
	}

	if(!T.bBlockingHit)
		return T.TraceEnd;
	else
		return T.TraceStart + T.Time * (T.TraceEnd - T.TraceStart) + (T.Normal * Underclip);
}

void UDefragrPlayerCollisionComponent::VectorMA(FVector Start, float Scale, FVector End, FVector& Out)
{
	Out.X = Start.X + Scale * End.X;
	Out.Y = Start.Y + Scale * End.Y;
	Out.Z = Start.Z + Scale * End.Z;
}