// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Templates/SharedPointer.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SMultiLineEditableConfigBox.h"

class FConfig;
class ISlateStyle;
class UConfigAsset;


/**
 * Implements the UConfigAsset asset editor widget.
 */
class SConfigAssetEditor
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SConfigAssetEditor) { }
	SLATE_END_ARGS()

public:

	/** Virtual destructor. */
	virtual ~SConfigAssetEditor();

	/**
	 * Construct this widget
	 *
	 * @param InArgs The declaration data for this widget.
	 * @param InConfigAsset The UConfigAsset asset to edit.
	 * @param InStyleSet The style set to use.
	 */
	void Construct(const FArguments& InArgs, UConfigAsset* InConfigAsset, const TSharedRef<ISlateStyle>& InStyle);

private:

	/** Callback for text changes in the editable text box. */
	void HandleEditableConfigBoxConfigChanged(const FConfig& NewConfig);

	/** Callback for committed text in the editable text box. */
	void HandleEditableConfigBoxConfigCommitted(const FConfig& Comment, EConfigCommit::Type CommitType);

	/** Callback for property changes in the text asset. */
	void HandleConfigAssetPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent);

private:

	/** Holds the editable text box widget. */
	TSharedPtr<SMultiLineEditableConfigBox> EditableConfigBox;

	/** Pointer to the text asset that is being edited. */
	UConfigAsset* ConfigAsset;
};
