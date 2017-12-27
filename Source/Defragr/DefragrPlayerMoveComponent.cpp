/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#include "DefragrPlayerMoveComponent.h"
#include "Defragr.h"
#include "DefragrPlayer.h"
#include "DefragrPlayerCollisionComponent.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformMath.h"

#include "Engine.h"


// Sets default values for this component's properties
UDefragrPlayerMoveComponent::UDefragrPlayerMoveComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UDefragrPlayerMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	Origin = Player->GetTransform().GetLocation();
}

// Called every frame
void UDefragrPlayerMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Delta = DeltaTime;

	if(Player)
	{
		// Grab the input from the player
		WishMove = Player->ConsumeMovementInput();

		if(MovementType == EMovementType::Spectate)
		{
			FlyMove();
			Player->Collider->SetWorldLocation(Origin);
			return;
		}

		QueueJump();

		CollisionComponent->TraceGround();

		if(CollisionComponent->CanGroundMove)
			GroundMove();
		else
			AirMove();

		CollisionComponent->TraceGround();

		// Record the ground velocity and speed of the player
		GroundVelocity.X = PlayerVelocity.X;
		GroundVelocity.Y = PlayerVelocity.Y;
		GroundSpeed = GroundVelocity.Size();

		Player->Collider->SetWorldLocation(Origin);

		// Logs
		/*GEngine->AddOnScreenDebugMessage(20, 0.01f, FColor::Red, FString::Printf(TEXT("Position [X: %.6f, Y: %.6f, Z: %.6f]"), GetOrigin().X, GetOrigin().Y, GetOrigin().Z));
		GEngine->AddOnScreenDebugMessage(2, 0.01f, FColor::Green, FString::Printf(TEXT("Is Grounded [%d]"), CollisionComponent->IsGrounded));
		GEngine->AddOnScreenDebugMessage(0, 0.01f, FColor::Green, FString::Printf(TEXT("Desired Velocity [X: %.2f, Y: %.2f, Z: %.2f]"), PlayerVelocity.X, PlayerVelocity.Y, PlayerVelocity.Z));
		GEngine->AddOnScreenDebugMessage(1, 0.01f, FColor::Green, FString::Printf(TEXT("Desired Linear Speed [%.2fups]"), GroundSpeed));*/
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 0.01f, FColor::Red, TEXT("NO OWNER"));
	}
}

void UDefragrPlayerMoveComponent::ApplyFriction()
{
	float speed;
	float newSpeed;
	float control;
	float drop = 0.0f;

	speed = PlayerVelocity.Size();

	if(CollisionComponent->IsGrounded)
	{
		control = speed < GroundAcceleration ? GroundDampening : speed;
		drop = control * Friction * Delta;
	}

	if(MovementType == EMovementType::Spectate)
		drop += speed * SpectatorFriction * Delta;

	newSpeed = speed - drop;
	if(newSpeed < 0.f)
		newSpeed = 0.f;
	if(speed > 0.f)
		newSpeed /= speed;

	PlayerVelocity.X *= newSpeed;
	PlayerVelocity.Y *= newSpeed;
}

void UDefragrPlayerMoveComponent::GroundMove()
{
	if(CheckJump())
	{
		AirMove();
		return;
	}

	ApplyFriction();

	FVector wishDirection;
	FVector wishvel = FVector::ZeroVector;

	FVector TransformForward = Player->ForwardVector;
	FVector TransformRight   = Player->RightVector;

	wishvel.X = TransformForward.X * WishMove.X + TransformRight.X * WishMove.Y;
	wishvel.Y = TransformForward.Y * WishMove.X + TransformRight.Y * WishMove.Y;
	wishvel.Z = TransformForward.Z * WishMove.X + TransformRight.Z * WishMove.Y;

	wishDirection = wishvel;
	float wishSpeed = wishDirection.Size();
	wishDirection.Normalize();
	wishSpeed *= GroundMaxSpeed;

	ApplyAcceleration(wishDirection, wishSpeed, GroundAcceleration);

	float vel = PlayerVelocity.Size();

	ClipVelocity(PlayerVelocity, CollisionComponent->GroundTrace.ImpactNormal, PlayerVelocity, CollisionComponent->Overclip);

	PlayerVelocity.Normalize();
	PlayerVelocity *= vel;

	// Don't do anything if standing still
	if(PlayerVelocity.X == 0.f && PlayerVelocity.Y == 0.f)
		return;

	CollisionComponent->StepSlideMove(false);
}

void UDefragrPlayerMoveComponent::AirMove()
{
	FVector wishDirection;
	FVector currentVelocity;
	float dynamicAcceleration;

	//ApplyFriction();

	// Set target direction for the character body to its inital state.
	wishDirection.X = Player->ForwardVector.X * WishMove.X + Player->RightVector.X * WishMove.Y;
	wishDirection.Y = Player->ForwardVector.Y * WishMove.X + Player->RightVector.Y * WishMove.Y;
	wishDirection.Z = 0.f;

	float wishSpeed = wishDirection.Size();
	wishDirection.Normalize();
	wishSpeed *= AirMaxSpeed;

	// CPM: Aircontrol
	float wishSpeed2 = wishSpeed;
	if (FVector::DotProduct(PlayerVelocity, wishDirection) < 0)
		dynamicAcceleration = AirStopAccelerate;
	if (WishMove.X == 0.f && WishMove.Y != 0.0f)
	{
		if (wishSpeed > AirStrafeSpeed)
			wishSpeed = AirStrafeSpeed;
		dynamicAcceleration = AirStrafeAcceleration;
	}

	ApplyAcceleration(wishDirection, wishSpeed, dynamicAcceleration);
	AirControl(wishDirection, wishSpeed);

	// Apply gravity
	PlayerVelocity.Z -= Gravity * Delta;

	CollisionComponent->StepSlideMove(true);
}

void UDefragrPlayerMoveComponent::AirControl(FVector WishDirection, float WishSpeed)
{
	float	zspeed, speed, dot, k;

	// Can't control movement if not moving forward or backward
	if(WishMove.X != 0.f || WishSpeed == 0.f)
		return;

	zspeed = PlayerVelocity.Z;
	PlayerVelocity.Z = 0;
	speed = PlayerVelocity.Size();
	PlayerVelocity.Normalize();

	dot = FVector::DotProduct(PlayerVelocity, WishDirection);
	k = 32.f;
	k *= CPMAirControl * dot * dot * Delta;

	// We can't change direction while slowing down
	if(dot > 0)
	{
		PlayerVelocity.X = PlayerVelocity.X * speed + WishDirection.X * k;
		PlayerVelocity.Y = PlayerVelocity.Y * speed + WishDirection.Y * k;
		PlayerVelocity.Normalize();
	}

	PlayerVelocity.X *= speed;
	PlayerVelocity.Y *= speed;
	PlayerVelocity.Z  = zspeed;
}

void UDefragrPlayerMoveComponent::ApplyAcceleration(FVector WishDirection, float WishSpeed, float DynamicAcceleration)
{
	float AddSpeed;
	float AccelerationSpeed;
	float CurrentSpeed;

	CurrentSpeed = FVector::DotProduct(WishDirection, PlayerVelocity);
	AddSpeed = WishSpeed - CurrentSpeed;
	if(AddSpeed <= 0.f)
		return;
	AccelerationSpeed = DynamicAcceleration * Delta * WishSpeed;
	if(AccelerationSpeed > AddSpeed)
		AccelerationSpeed = AddSpeed;

	PlayerVelocity.X += AccelerationSpeed * WishDirection.X;
	PlayerVelocity.Y += AccelerationSpeed * WishDirection.Y;
	PlayerVelocity.Z += AccelerationSpeed * WishDirection.Z;
}

void UDefragrPlayerMoveComponent::FlyMove()
{
	float	speed, drop, friction, control, newspeed;
	FVector		wishvel;
	float		fmove, smove;
	FVector		wishdir;
	float		wishspeed;
//	float		scale;

	// Set the view height
	//pm->ps->viewheight = DEFAULT_VIEWHEIGHT;

	// friction

	speed = PlayerVelocity.Size();
	if(speed < 1)
	{
		PlayerVelocity = FVector::ZeroVector;
	}
	else
	{
		drop = 0;

		friction = Friction * 1.5;	// extra friction
		control = speed < SpectatorStopSpeed ? SpectatorStopSpeed : speed;
		drop += control*friction * Delta;

		// scale the velocity
		newspeed = speed - drop;
		if (newspeed < 0)
			newspeed = 0;
		newspeed /= speed;

		PlayerVelocity *= newspeed;
	}

	// accelerate
	//scale = PM_CmdScale(&pm->cmd);

	fmove = WishMove.X;
	smove = WishMove.Y;

	wishvel.X = Player->ForwardVector.X * fmove + Player->RightVector.X * smove;
	wishvel.Y = Player->ForwardVector.Y * fmove + Player->RightVector.Y * smove;
	wishvel.Z += 0.f;

	wishdir = wishvel;
	wishspeed = wishdir.Size();
	wishdir.Normalize();
	wishspeed *= 1000.f;

	ApplyAcceleration(wishdir, wishspeed, SpectatorAccelerate);

	// move
	Player->CollisionComponent->VectorMA(GetOrigin(), Delta, PlayerVelocity, Origin);
	Origin.Z = 0.f;
}

bool UDefragrPlayerMoveComponent::CheckJump()
{
	if(WishJump)
	{
		PlayerVelocity.Z = JumpForce;
		Player->PlayJumpSound();

		WishJump = false;

		return true;
	}

	return false;
}

void UDefragrPlayerMoveComponent::QueueJump()
{
	if(Player->JumpInput && !WishJump)
		WishJump = true;
	if(!Player->JumpInput)
		WishJump = false;
}

float UDefragrPlayerMoveComponent::CmdScale()
{
	int		max;
	float	total;
	float	scale;

	max = FGenericPlatformMath::Abs(WishMove.X);
	if (FGenericPlatformMath::Abs(WishMove.Y) > max) {
		max = FGenericPlatformMath::Abs(WishMove.Y);
	}
	/*if (abs(cmd->upmove) > max) {
		max = abs(cmd->upmove);
	}*/
	if (!max) {
		return 0;
	}

	total = sqrt(WishMove.X * WishMove.X + WishMove.Y * WishMove.Y);
	scale = (float)GroundSpeed * max / (127.0 * total);

	return scale;
}

void UDefragrPlayerMoveComponent::ClipVelocity(FVector In, FVector Normal, FVector& Out, float Overbounce)
{
	float Backoff = FVector::DotProduct(In, Normal);

	if (Backoff < 0.f)
		Backoff *= Overbounce;
	else
		Backoff /= Overbounce;

	Out.X = In.X - (Normal.X * Backoff);
	Out.Y = In.Y - (Normal.Y * Backoff);
	Out.Z = In.Z - (Normal.Z * Backoff);
}



void UDefragrPlayerMoveComponent::SetOrigin(FVector Position)
{
	Origin = Position;
}

FVector UDefragrPlayerMoveComponent::GetOrigin()
{
	return Origin;
}