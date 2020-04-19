// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Internationalization/Config.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"

#include "ConfigAsset.generated.h"


/**
 * Implements an asset that can be used to store arbitrary text, such as notes
 * or documentation.
 */
UCLASS(BlueprintType, hidecategories=(Object))
class TEXTASSET_API UConfigAsset
	: public UObject
{
	GENERATED_BODY()

public:

	/** Holds the stored text. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="ConfigAsset")
	FConfig Config;
};
