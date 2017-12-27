/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#pragma once

#include "GameFramework/Pawn.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"

#include "DefragrPlayer.generated.h"

class UDefragrPlayerMoveComponent;
class UDefragrPlayerCollisionComponent;
class UPrimitiveComponent;
class AWeapon;


UCLASS(config = Game, BlueprintType, hideCategories = ("Pawn|DefragrCharacter|InternalEvents"), meta = (ShortTooltip = "Custom character specifically built for Q3 movement."))
class ADefragrPlayer : public APawn
{
	GENERATED_BODY()

public:
	ADefragrPlayer();

	/** Called when the character changes controller or gets a new one */
	virtual void PossessedBy(AController* NewController);

	/** Called when the controller sets up the input for the pawn */
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Per frame tick */
	virtual void Tick(float Delta);

	/** Holds the controller that's possessing this character */
	AController* Controller;

public:
	/* Health of the player */
	UPROPERTY(Category = "General", EditAnywhere, BlueprintReadWrite)
	int32 Health = 100;

	/* Armour of the player */
	UPROPERTY(Category = "General", EditAnywhere, BlueprintReadWrite)
	int32 Armour = 0;

public:
	UPROPERTY(Category = "Weapons", EditAnywhere, BlueprintReadWrite)
	TArray<AWeapon*> Slots;

	/* The currently active slot the player is using */
	UPROPERTY(Category = "Weapons", EditAnywhere, BlueprintReadWrite)
	uint8 ActiveSlot = 0;

	/* Force player to swap to the next weapon in his inventory */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void NextWeapon();
	
	/* Force player to swap to the previous weapon in his inventory */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PreviousWeapon();

	/* Force player to swap to the specified slot ID */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Weapon(uint8 SlotID);

	/* True when the player needs to change weapons, read on tick */
	UPROPERTY(Category = "Weapons", VisibleAnywhere, BlueprintReadOnly)
	bool ChangeWeapon = false;

	/* The slot ID to change to when ChangeWeapon is true, read on tick */
	UPROPERTY(Category = "Weapons", VisibleAnywhere, BlueprintReadOnly)
	uint8 ChangeToSlot = 0;

public:
	void PickupWeapon(TSubclassOf<AWeapon> Weapon);

public:
	UPROPERTY(Category = "Sounds", EditAnywhere, BlueprintReadWrite)
	USoundCue* JumpSoundCue;

	UPROPERTY(Category = "Sounds", EditAnywhere, BlueprintReadWrite)
	USoundCue* HardLandingSoundCue;

	UPROPERTY(Category = "Sounds", EditAnywhere, BlueprintReadWrite)
	USoundCue* TeleportSoundCue;

	void PlayJumpSound();
	void PlayTeleportSound();

public:
	FTimerHandle HRaceTimeTick;

	/* Current race time in milliseconds */
	int32 RaceTime = 0;

	/* Fastest lap time in milliseconds */
	int32 FastestRaceTime = 0;

	/* Checkpoint split times in milliseconds */
	TArray<int32> RaceSplitTimes;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartRaceTimer();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopRaceTimer();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void ResetRaceTimer();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	int32 GetRaceTime();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	FString GetRaceTimeString();

	/* Called from a timer that's set for every millisecond */
	void RaceTimeTick();

public:
	/* Sets the position of the player */
	UFUNCTION(BlueprintCallable, Category = "DefragrCharacter")
	void SetPosition(FVector NewPosition);

	/* Gets the position of the player */
	UFUNCTION(BlueprintCallable, Category = "DefragrCharacter")
	FVector GetPosition();

	/* Sets the velocity of the player */
	UFUNCTION(BlueprintCallable, Category = "DefragrCharacter")
	void SetVelocity(FVector NewVelocity);

	/* Sets the velocity in relation to the player */
	UFUNCTION(BlueprintCallable, Category = "DefragrCharacter")
	void SetRelativeVelocity(FVector NewVelocity);

	/* Gets the velocity of the player */
	UFUNCTION(BlueprintCallable, Category = "DefragrCharacter")
	virtual FVector GetVelocity() const override; // Need override here because APawn has this function as well

	/* Sets the player's viewing angles */
	UFUNCTION(BlueprintCallable, Category = "DefragrCharacter")
	void SetRotation(FQuat NewRotation);

	/* Gets the player's viewing angles */
	UFUNCTION(BlueprintCallable, Category = "DefragrCharacter")
	FRotator GetRotation();

	/* Get the player's linear speed */
	UFUNCTION(BlueprintCallable, Category = "DefragrCharacter")
	float GetGroundSpeed();

	/* Commit to a jump */
	UFUNCTION(BlueprintCallable, Category = "DefragrCharacter|CharacterMovement")
	virtual void DoJump();

	UFUNCTION(BlueprintCallable, Category = "DefragrCharacter|CharacterMovement")
	virtual void StopJump();

public:
	/* The local normalized forward vector of the player's facing angle */
	UPROPERTY(Category = Transform, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector ForwardVector;

	/* The local normalized right vector of the player's facing angle */
	UPROPERTY(Category = Transform, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector RightVector;

	/* X is forward [W] and back [S] (1.0 to -1.0) and Y is right [D] and left [A] (1.0 to - 1.0) */
	UPROPERTY(Category = Input, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D MovementInput;

	UPROPERTY(Category = Input, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool JumpInput;

	/* Stores relational (to last frame) mouse position */
	FVector2D MouseVelocity;

public:
	/** The BoxComponent being used for movement collision. Always treated as being vertically aligned in simple collision check functions */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Collider;
	
	/** Character's movement component */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDefragrPlayerMoveComponent* MovementComponent;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDefragrPlayerCollisionComponent* CollisionComponent;

	USceneComponent* PlayerForwardRefComponent;

	/** Helper object to see which direction is forward */
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UArrowComponent* ArrowComponent;
#endif

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	/** Takes mouse input and converts it into camera rotational movements */
	void UpdateViewingAngles();

	/** Handles moving forward/backward */
	void MoveForward(float Value);

	/** Handles strafing movement, left and right */
	void MoveRight(float Value);

	/** Mouse movement input callbacks */
	void MouseX(float Value);
	void MouseY(float Value);

	/* Scroll wheel input callback */
	void ScrollUp();
	void ScrollDown();

	/**
	 * Gets and immediately consumes the movement input where 
	 * X is forward [W] and back [S] (1.0 to -1.0) and Y is right [D] and left [A] (1.0 to - 1.0)
	 */
	FVector2D ConsumeMovementInput();

	/** Gets and immediately consumes mouse input */
	FVector2D ConsumeMouseInput();
	
};
