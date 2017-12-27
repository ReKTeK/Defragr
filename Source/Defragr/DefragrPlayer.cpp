/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#include "DefragrPlayer.h"
#include "Defragr.h"
#include "DefragrPlayerMoveComponent.h"
#include "DefragrPlayerCollisionComponent.h"
#include "Weapon.h"

#include "Engine.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"


ADefragrPlayer::ADefragrPlayer()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		FName ID_Characters;
		FText NAME_Characters;
		FConstructorStatics()
			: ID_Characters(TEXT("Characters"))
			, NAME_Characters(NSLOCTEXT("SpriteCategory", "Characters", "Characters"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create the player collision
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Player Collision"));
	Collider->InitBoxExtent(FVector(30.f, 30.f, 56.f));

	// Add physical reference to the player's forward vector
	PlayerForwardRefComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Player Forward"));
	if(PlayerForwardRefComponent)
		PlayerForwardRefComponent->SetupAttachment(Collider);

	// Add a helper
#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if(ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SpriteInfo.Category = ConstructorStatics.ID_Characters;
		ArrowComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_Characters;
		ArrowComponent->SetupAttachment(PlayerForwardRefComponent);
		ArrowComponent->bIsScreenSizeScaled = true;
	}
#endif

	// Initialize the camera
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(PlayerForwardRefComponent);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 56.f - 4.f));

	// Add the character collision and movement component
	CollisionComponent = CreateDefaultSubobject<UDefragrPlayerCollisionComponent>(TEXT("Defragr Character Collision"));
	MovementComponent = CreateDefaultSubobject<UDefragrPlayerMoveComponent>(TEXT("Defragr Character Movement"));

	if(CollisionComponent)
	{
		CollisionComponent->Player = this;
		CollisionComponent->MovementComponent = MovementComponent;
	}

	// Throw down the character movement
	if(MovementComponent)
	{
		MovementComponent->UpdatedComponent = Collider;

		MovementComponent->CollisionComponent = CollisionComponent;
		MovementComponent->Player = this;
	}

	// Initialize the weapon slots
	Slots.Init(NULL, 10);
}

void ADefragrPlayer::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	check(InputComponent);

	// Set up movement
	InputComponent->BindAxis("MoveForward", this, &ADefragrPlayer::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ADefragrPlayer::MoveRight);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ADefragrPlayer::DoJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ADefragrPlayer::StopJump);

	// Set up mouse inputs
	InputComponent->BindAxis("Turn", this, &ADefragrPlayer::MouseX);
	InputComponent->BindAxis("LookUp", this, &ADefragrPlayer::MouseY);

	// Set up scroll input
	InputComponent->BindAction("ScrollUp", IE_Pressed, this, &ADefragrPlayer::ScrollUp);
	InputComponent->BindAction("ScrollDown", IE_Pressed, this, &ADefragrPlayer::ScrollDown);
}

void ADefragrPlayer::Tick(float DeltaTime)
{
	// Handles the input from mouse to control the camera
	UpdateViewingAngles();

	// After we decide where the player is facing, we get the forward vector
	// of the player for easy access.
	ForwardVector = PlayerForwardRefComponent->GetForwardVector();
	RightVector   = PlayerForwardRefComponent->GetRightVector();

	if(Health <= 0)
	{
		Health = 0;
	}
}

void ADefragrPlayer::NextWeapon()
{
	ChangeToSlot++;

	if(ChangeToSlot > 9)
		ChangeToSlot = 9;

	ChangeWeapon = true;
}

void ADefragrPlayer::PreviousWeapon()
{
	ChangeToSlot--;

	if(ChangeToSlot < 0)
		ChangeToSlot = 0;

	ChangeWeapon = true;
}

void ADefragrPlayer::Weapon(uint8 SlotID)
{
	ChangeToSlot = SlotID;
	ChangeWeapon = true;
}

void ADefragrPlayer::PickupWeapon(TSubclassOf<AWeapon> Weapon)
{
	AWeapon* DefaultWeapon = Weapon->GetDefaultObject<AWeapon>();

	if(DefaultWeapon == NULL)
		return;

	// Ensure we have a valid slot ID for the weapon
	if(DefaultWeapon->SlotID > 9)
		return;

	// If there is no weapon described here, then add it
	if(Slots[DefaultWeapon->SlotID] == NULL)
	{
		Slots[DefaultWeapon->SlotID] = DefaultWeapon;
		return;
	}
	
	// When we get to this point, there is a weapon already in the slot
	// if it's the same type of weapon then just call the extra pickup
	// otherwise just re-assign the weapon slot.
	if(Slots[DefaultWeapon->SlotID]->WeaponType == DefaultWeapon->WeaponType)
	{
		Slots[DefaultWeapon->SlotID]->ExtraPickup();
		return;
	}

	Slots[DefaultWeapon->SlotID] = DefaultWeapon;
}

void ADefragrPlayer::UpdateViewingAngles()
{
	// Pitch camera up and down
	if (FirstPersonCameraComponent)
	{
		FVector v(0.f, -MouseVelocity.Y, 0.f);
		FirstPersonCameraComponent->AddLocalRotation(FQuat::MakeFromEuler(v));
	}

	// Rotate the reference point component
	if (PlayerForwardRefComponent)
	{
		FVector v(0.f, 0.f, MouseVelocity.X);
		PlayerForwardRefComponent->AddWorldRotation(FQuat::MakeFromEuler(v));
	}
}

void ADefragrPlayer::PossessedBy(AController* NewController)
{
	// Assign the new controller
	Controller = NewController;
}

void ADefragrPlayer::StartRaceTimer()
{
	GetWorldTimerManager().SetTimer(HRaceTimeTick, this, &ADefragrPlayer::RaceTimeTick, 0.001f, true);
}

void ADefragrPlayer::StopRaceTimer()
{
	GetWorldTimerManager().ClearTimer(HRaceTimeTick);
}

void ADefragrPlayer::ResetRaceTimer()
{
	RaceTime = 0;
	StopRaceTimer();
}

int32 ADefragrPlayer::GetRaceTime()
{
	return RaceTime;
}

FString ADefragrPlayer::GetRaceTimeString()
{
	const int32 Minutes      = FMath::FloorToInt((RaceTime / 1000) / 60);
	const int32 Seconds      = FMath::FloorToInt((RaceTime / 1000) % 60);
	const int32 Milliseconds = RaceTime % 1000;

	if(Minutes > 0)
		return FString::Printf(TEXT("%d:%d.%03d"), Minutes, Seconds, Milliseconds);
	else
		return FString::Printf(TEXT("%d.%03d"), Seconds, Milliseconds);
}

void ADefragrPlayer::RaceTimeTick()
{
	RaceTime++;
}

void ADefragrPlayer::PlayJumpSound()
{
	UGameplayStatics::PlaySoundAtLocation(this, JumpSoundCue, GetTransform().GetLocation());
}

void ADefragrPlayer::PlayTeleportSound()
{
	UGameplayStatics::PlaySoundAtLocation(this, TeleportSoundCue, GetTransform().GetLocation());
}

void ADefragrPlayer::SetPosition(FVector NewPosition)
{
	MovementComponent->Origin = NewPosition;
}

FVector ADefragrPlayer::GetPosition()
{
	return MovementComponent->GetOrigin();
}

void ADefragrPlayer::SetVelocity(FVector NewVelocity)
{
	MovementComponent->PlayerVelocity = NewVelocity;
}

void ADefragrPlayer::SetRelativeVelocity(FVector NewVelocity)
{
	MovementComponent->PlayerVelocity = PlayerForwardRefComponent->GetRelativeTransform().TransformVector(NewVelocity);
}

FVector ADefragrPlayer::GetVelocity() const
{
	return this->MovementComponent->PlayerVelocity;
}

void ADefragrPlayer::SetRotation(FQuat NewRotation)
{
	// We only need the pitch of the camera since
	// the camera is attached to the player's body
	// and the body does the Z rotation
	FRotator NewCameraRotation(NewRotation);
	NewCameraRotation.Roll = 0.f;
	NewCameraRotation.Yaw  = 0.f;
	FirstPersonCameraComponent->SetRelativeRotation(NewCameraRotation);
	

	// Squash the roll and pitch of the rotation for the Z
	// rotation of the actual player.
	FRotator NewPlayerRotation(NewRotation);
	NewPlayerRotation.Roll  = 0.f;
	NewPlayerRotation.Pitch = 0.f;
	PlayerForwardRefComponent->SetWorldRotation(NewPlayerRotation);
}

FRotator ADefragrPlayer::GetRotation()
{
	// Since the perceived rotation of the player is actually
	// split between the camera and a helper, we have to
	// combine them.
	return FirstPersonCameraComponent->GetComponentRotation();
}

float ADefragrPlayer::GetGroundSpeed()
{
	return MovementComponent->GroundSpeed;
}

void ADefragrPlayer::DoJump()
{
	JumpInput = true;
}

void ADefragrPlayer::StopJump()
{
	JumpInput = false;
}

void ADefragrPlayer::MoveForward(float Value)
{
	if (Value != 0.0f)
		MovementInput.X = Value;
}

void ADefragrPlayer::MoveRight(float Value)
{
	if (Value != 0.0f)
		MovementInput.Y = Value;
}

void ADefragrPlayer::MouseX(float Value)
{
	MouseVelocity.X = Value;
}

void ADefragrPlayer::MouseY(float Value)
{
	MouseVelocity.Y = Value;
}

void ADefragrPlayer::ScrollUp()
{
	NextWeapon();
}

void ADefragrPlayer::ScrollDown()
{
	PreviousWeapon();
}

FVector2D ADefragrPlayer::ConsumeMovementInput()
{
	FVector2D LastWishMove = MovementInput;
	MovementInput = FVector2D::ZeroVector;

	return LastWishMove;
}

FVector2D ADefragrPlayer::ConsumeMouseInput()
{
	FVector2D LastRelationalMousePos = MouseVelocity;
	MouseVelocity = FVector2D::ZeroVector;

	return MouseVelocity;
}