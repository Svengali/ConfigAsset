// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ConfigAssetFactoryNew.h"

#include "ConfigAsset.h"


/* UConfigAssetFactoryNew structors
 *****************************************************************************/

UConfigAssetFactoryNew::UConfigAssetFactoryNew(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UConfigAsset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}


/* UFactory overrides
 *****************************************************************************/

UObject* UConfigAssetFactoryNew::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UConfigAsset>(InParent, InClass, InName, Flags);
}


bool UConfigAssetFactoryNew::ShouldShowInNewMenu() const
{
	return true;
}
