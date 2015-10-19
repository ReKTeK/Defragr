// Copyright Terence-Lee 'Zinglish' Davis.

#pragma once

// BluEye Classes
#include "ModuleManager.h"
#include "IBlu.h"
#include "BluManager.h"
#include "BluEye.h"
#include "RenderHandler.h"
#include "BluJsonObj.h"
#include "BluBlueprintFunctionLibrary.h"

#include "Object.h"
#include "BluAPIManager.generated.h"


UCLASS()
class DEFRAGR_API UBluAPIManager : public UObject
{
	GENERATED_BODY()

public:
	/* Retrieves an instance of BluiAPI */
	UFUNCTION(Category = "Blu API Manager", BlueprintCallable)
	static UBluAPIManager* GetInstance();

	UFUNCTION(Category = "Blu API Manager", BlueprintCallable)
	void SetBlu(UBluEye* BluEyeRef);

	UFUNCTION(Category = "Blu API Manager", BlueprintCallable)
	void ParseGenericScriptEvent(FString EventName, FString JSONString);

protected:
	static UBluAPIManager* Instance;
	static UBluEye* BluEye;

private:
	UBluAPIManager() {};
};