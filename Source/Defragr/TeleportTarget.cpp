/* Copyright (C) Terence-Lee 'Zinglish' Davis
 * Written by Terence-Lee 'Zinglish' Davis <zinglish[at]gmail.com>
 */

#include "TeleportTarget.h"
#include "Defragr.h"

#include "Components/BillboardComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ATeleportTarget::ATeleportTarget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		// A helper class object we use to find target UTexture2D object in resource package
		ConstructorHelpers::FObjectFinderOptional<UTexture2D> TextureObject;

		// Icon sprite category name
		FName ID_TeleportTarget;

		// Icon sprite display name
		FText NAME_TeleportTarget;

		FConstructorStatics()
			: TextureObject(TEXT("/Game/Defragr/EditorResources/TeleportTarget"))
			, ID_TeleportTarget(TEXT("TeleportTarget"))
			, NAME_TeleportTarget(NSLOCTEXT("SpriteCategory", "TeleportTarget", "TeleportTarget"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// We need a scene component to attach Icon sprite
	USceneComponent* SceneComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneComp"));
	RootComponent = SceneComponent;
	RootComponent->Mobility = EComponentMobility::Static;

#if WITH_EDITORONLY_DATA
	SpriteComponent = ObjectInitializer.CreateEditorOnlyDefaultSubobject<UBillboardComponent>(this, TEXT("Sprite"));
	if(SpriteComponent)
	{
		SpriteComponent->Sprite                 = ConstructorStatics.TextureObject.Get();	// Get the sprite texture from helper class object
		SpriteComponent->SpriteInfo.Category    = ConstructorStatics.ID_TeleportTarget;		// Assign sprite category name
		SpriteComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_TeleportTarget;	// Assign sprite display name
		SpriteComponent->SetupAttachment(RootComponent);							// Attach sprite to scene component
		SpriteComponent->Mobility               = EComponentMobility::Static;
		SpriteComponent->bIsScreenSizeScaled    = true;
	}

	ArrowComponent =  ObjectInitializer.CreateEditorOnlyDefaultSubobject<UArrowComponent>(this, TEXT("Arrow"));
	if(ArrowComponent)
	{
		ArrowComponent->ArrowColor             = FColor(150, 200, 255);
		ArrowComponent->bTreatAsASprite        = true;
		ArrowComponent->SpriteInfo.Category    = ConstructorStatics.ID_TeleportTarget;
		ArrowComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_TeleportTarget;
		ArrowComponent->SetupAttachment(RootComponent);
		ArrowComponent->bIsScreenSizeScaled    = true;
	}

	PlayerExtents = CreateDefaultSubobject<UBoxComponent>(TEXT("Player Extents"));
	PlayerExtents->InitBoxExtent(FVector(30.f, 30.f, 56.f));
	PlayerExtents->SetRelativeLocation(FVector::ZeroVector);
#endif // WITH_EDITORONLY_DATA
}

