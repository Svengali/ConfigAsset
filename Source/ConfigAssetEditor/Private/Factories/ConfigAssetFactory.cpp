// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ConfigAssetFactory.h"

#include "Containers/UnrealString.h"
#include "ConfigAsset.h"
#include "Misc/FileHelper.h"


/* UConfigAssetFactory structors
 *****************************************************************************/

UConfigAssetFactory::UConfigAssetFactory( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{
	Formats.Add(FString(TEXT("txt;")) + NSLOCTEXT("UConfigAssetFactory", "FormatTxt", "Config File").ToString());
	SupportedClass = UConfigAsset::StaticClass();
	bCreateNew = false;
	bEditorImport = true;
}


/* UFactory overrides
 *****************************************************************************/

/* This is the old API (only for demonstration purposes)
UObject* UConfigAssetFactory::FactoryCreateBinary(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn)
{
	UConfigAsset* ConfigAsset = nullptr;
	FString ConfigString;

	if (FFileHelper::LoadFileToString(ConfigString, *CurrentFilename))
	{
		ConfigAsset = NewObject<UConfigAsset>(InParent, Class, Name, Flags);
		ConfigAsset->Config = FConfig::FromString(ConfigString);
	}

	return ConfigAsset;
}*/


UObject* UConfigAssetFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	UConfigAsset* ConfigAsset = nullptr;
	FString ConfigString;

	if (FFileHelper::LoadFileToString(ConfigString, *Filename))
	{
		ConfigAsset = NewObject<UConfigAsset>(InParent, InClass, InName, Flags);
		ConfigAsset->Config = FConfig::FromString(ConfigString);
	}

	bOutOperationCanceled = false;

	return ConfigAsset;
}
