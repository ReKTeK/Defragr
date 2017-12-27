/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Pickup.generated.h"

class ADefragrPlayer;


UCLASS(Blueprintable)
class DEFRAGR_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	APickup();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

public:
	virtual void OnPlayerPickup(ADefragrPlayer* Player);

public:
	/* The pickup area */
	UPROPERTY(Category = "Pickup", VisibleDefaultsOnly, BlueprintReadOnly)
	UBoxComponent* BaseCollisionComponent;

	/* Static mesh to represent the pickup */
	UPROPERTY(Category = "Pickup", VisibleDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* PickupMesh;

public:
	/* True if active and able to be picked up */
	UPROPERTY(Category = "Pickup", EditAnywhere, BlueprintReadWrite)
	bool IsActive = true;

	/* Respawn time in seconds */
	UPROPERTY(Category = "Pickup", EditAnywhere, BlueprintReadWrite)
	float RespawnTime = 30.0f;

public:
	/* Called when the pickup is taken by the player */
	UFUNCTION(BlueprintNativeEvent)
	void OnPickedUp();

private:
	UFUNCTION()
	void OnActorOverlapPickup(class UPrimitiveComponent* Comp, class AActor* Actor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
