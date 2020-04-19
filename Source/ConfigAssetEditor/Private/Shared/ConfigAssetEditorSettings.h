// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Fonts/SlateFontInfo.h"
#include "Styling/SlateColor.h"
#include "UObject/ObjectMacros.h"

#include "ConfigAssetEditorSettings.generated.h"


UCLASS(config=Editor)
class TEXTASSETEDITOR_API UConfigAssetEditorSettings
	: public UObject
{
	GENERATED_BODY()

public:

	/** Color of the ConfigAsset editor's background. */
	UPROPERTY(config, EditAnywhere, Category=Appearance)
	FSlateColor BackgroundColor;

	/** Color of the ConfigAsset editor's text. */
	UPROPERTY(config, EditAnywhere, Category=Appearance)
	FSlateColor ForegroundColor;

	/** The font to use in the ConfigAsset editor window. */
	UPROPERTY(config, EditAnywhere, Category=Appearance)
	FSlateFontInfo Font;

	/** The margin around the ConfigAsset editor window (in pixels). */
	UPROPERTY(config, EditAnywhere, Category=Appearance)
	float Margin;

public:

	/** Default constructor. */
	UConfigAssetEditorSettings();
};
