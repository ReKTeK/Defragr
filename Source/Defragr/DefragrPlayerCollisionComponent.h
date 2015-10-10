/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#pragma once

#include "Components/ActorComponent.h"
#include "DefragrPlayerCollisionComponent.generated.h"

class ADefragrPlayer;
class UDefragrPlayerMoveComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEFRAGR_API UDefragrPlayerCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ADefragrPlayer* Player;
	UDefragrPlayerMoveComponent* MovementComponent;

	UDefragrPlayerCollisionComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	float Delta;
	FHitResult GroundTrace;

public:
	/** The amount the collider has to travel before detecting the ground */
	UPROPERTY(Category = "General Collision", EditAnywhere, BlueprintReadWrite)
	float GroundTraceDistance = 0.25f;

	UPROPERTY(Category = "General Collision", EditAnywhere, BlueprintReadWrite)
	float MinWalkNormal = 0.7f;

	UPROPERTY(Category = "General Collision", EditAnywhere, BlueprintReadWrite)
	float Overclip = 1.001f;

	// The amount of 'skin' the player has
	UPROPERTY(Category = "General Collision", EditAnywhere, BlueprintReadWrite)
	float Underclip = 0.1f;

	UPROPERTY(Category = "General Collision", EditAnywhere, BlueprintReadWrite)
	int32 MaxClipPlanes = 5;

	UPROPERTY(Category = "General Collision", EditAnywhere, BlueprintReadWrite)
	int32 MaxStepSize = 18;

public:
	/** Is the player touching the ground. This includes steep slopes and such */
	bool IsGrounded;

	/** Determines whether the player can run or not */
	bool CanGroundMove;

private:
	/** Uses the player's collider to trace from start to end */
	bool Trace(FHitResult& Result, FVector Start, FVector End);

	/** Moves a vector from Start to ENd, where Scale is the percent of movement (from 0.0 - 1.0) */
public:
	void VectorMA(FVector Start, float Scale, FVector End, FVector& Out);

/**
	The following methods make changes to the player internally as their intended output.
*/
public:
	/** Auto traces ground */
	void TraceGround();

	/** Gets the trace's ending. If there's a hit, it will return the location of the hit, if there
	 is no hit then the trace's ending will return. If the trace started in a collider then the
	 resolution vector will be returned */
	FVector TraceEnd(FHitResult& T);

	/** Attempts to solve a collision by sliding the player and clipping velocity */
	bool SlideMove(bool Gravity);

	/** Same as SlideMove except it takes into account slopes and stairs */
	void StepSlideMove(bool Gravity);
	
	void SolvePenetration(FHitResult& PenTrace);
};
