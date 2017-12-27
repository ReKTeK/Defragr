/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#include "Pickup.h"
#include "Defragr.h"

#include "Engine.h"

#include "DefragrPlayer.h"



// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the root component
	BaseCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BaseCollisionComponent"));
	BaseCollisionComponent->InitBoxExtent(FVector(30.f, 30.f, 56.f));
	RootComponent = BaseCollisionComponent;

	// Make the collider a trigger
	static FName CollisionProfileName(TEXT("Trigger"));
	BaseCollisionComponent->SetCollisionProfileName(CollisionProfileName);
	BaseCollisionComponent->bGenerateOverlapEvents = true;

	// Callback when an actor overlaps the pickup
	BaseCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnActorOverlapPickup);

	// Create the pickup mesh
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(RootComponent);
}

void APickup::BeginPlay() {Super::BeginPlay();}
void APickup::Tick(float DeltaTime) {Super::Tick(DeltaTime);}

void APickup::OnPlayerPickup(ADefragrPlayer* Player) {}
void APickup::OnPickedUp_Implementation() {}

void APickup::OnActorOverlapPickup(class UPrimitiveComponent* Comp, class AActor* Actor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ADefragrPlayer* Player = Cast<ADefragrPlayer>(Actor);

	if(Player)
	{
		OnPlayerPickup(Player);
		OnPickedUp();
	}
}
