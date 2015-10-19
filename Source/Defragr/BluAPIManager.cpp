// Copyright Terence-Lee 'Zinglish' Davis.

#include "Defragr.h"

#include "Json.h"
#include "FileManagerGeneric.h"

#include "BluAPIManager.h"


UBluAPIManager* UBluAPIManager::Instance = NULL;
UBluEye* UBluAPIManager::BluEye = NULL;

UBluAPIManager* UBluAPIManager::GetInstance()
{
	if(!Instance->IsValidLowLevel())
		Instance = NewObject<UBluAPIManager>(GetTransientPackage(), UBluAPIManager::StaticClass());

	return Instance;
}

void UBluAPIManager::SetBlu(UBluEye* BluEyeRef)
{
	this->BluEye = BluEyeRef;
}

void UBluAPIManager::ParseGenericScriptEvent(FString EventName, FString JSONString)
{
	GEngine->AddOnScreenDebugMessage(11, 5.0f, FColor::Green, JSONString);

	// Parse the input
	UBluJsonObj* InData = NewObject<UBluJsonObj>(GetTransientPackage(), UBluJsonObj::StaticClass());
	InData->init(JSONString);

	int callbackID = int(InData->getNumValue("callbackID"));

	// Set up output
	TSharedPtr<FJsonObject> OutData = MakeShareable(new FJsonObject);

	

	/* ListFiles() */
	if(EventName.Equals("GetFileList", ESearchCase::CaseSensitive))
	{
		GEngine->AddOnScreenDebugMessage(10, 5.0f, FColor::Black, EventName);

		// Prepare an array to insert into out data
		TArray<TSharedPtr<FJsonValue>> FileList;

		// Tear and prepare input
		FString Path = InData->getStringValue("path");
		//FPaths::NormalizeDirectoryName(Path);

		TArray<FString> Files;
		Files.Empty();
		if(FPaths::DirectoryExists(Path))
		{
			FString WildPath = Path + "*";
			FFileManagerGeneric::Get().FindFiles(Files, *WildPath, true, false);
			for(int i = 0; i < Files.Num(); i++)
			{
				// Add to out data array
				TSharedPtr<FJsonValueString> JsonFileName = MakeShareable(new FJsonValueString(Files[i]));
				FileList.Add(JsonFileName);
			}
		}

		OutData->SetArrayField("files", FileList);
	}


	// If a callback was specified then commit to executing it with the helper
	if(callbackID > -1)
	{
		// Serialize the out data
		FString OutputString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
		FJsonSerializer::Serialize(OutData.ToSharedRef(), Writer);

		// Create the parameters to execute the callback, then execute
		TArray<FString> MethodParams;
		MethodParams.Add(FString::FromInt(callbackID));
		MethodParams.Add(OutputString);
		BluEye->ExecuteJSMethodWithParams("BluAPIManager.callback", MethodParams);
	}

	delete InData;
}
