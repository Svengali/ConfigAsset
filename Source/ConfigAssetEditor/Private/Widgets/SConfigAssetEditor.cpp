// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SConfigAssetEditor.h"

#include "Fonts/SlateFontInfo.h"
#include "Internationalization/Text.h"
#include "ConfigAsset.h"
#include "UObject/Class.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

#include "ConfigAssetEditorSettings.h"


#define LOCTEXT_NAMESPACE "SConfigAssetEditor"


/* SConfigAssetEditor interface
 *****************************************************************************/

SConfigAssetEditor::~SConfigAssetEditor()
{
	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);
}


void SConfigAssetEditor::Construct(const FArguments& InArgs, UConfigAsset* InConfigAsset, const TSharedRef<ISlateStyle>& InStyle)
{
	ConfigAsset = InConfigAsset;

	auto Settings = GetDefault<UConfigAssetEditorSettings>();

	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SAssignNew(EditableTextBox, SMultiLineEditableTextBox)
					.BackgroundColor((Settings != nullptr) ? Settings->BackgroundColor : FLinearColor::White)
					.Font((Settings != nullptr) ? Settings->Font : FSlateFontInfo())
					.ForegroundColor((Settings != nullptr) ? Settings->ForegroundColor : FLinearColor::Black)
					.Margin((Settings != nullptr) ? Settings->Margin : 4.0f)
					.OnTextChanged(this, &SConfigAssetEditor::HandleEditableTextBoxTextChanged)
					.OnTextCommitted(this, &SConfigAssetEditor::HandleEditableTextBoxTextCommitted)
					.Text(ConfigAsset->Config)
			]
	];

	FCoreUObjectDelegates::OnObjectPropertyChanged.AddSP(this, &SConfigAssetEditor::HandleConfigAssetPropertyChanged);
}


/* SConfigAssetEditor callbacks
 *****************************************************************************/

void SConfigAssetEditor::HandleEditableTextBoxTextChanged(const FText& NewConfig)
{
	ConfigAsset->MarkPackageDirty();
}


void SConfigAssetEditor::HandleEditableTextBoxTextCommitted(const FText& Comment, ETextCommit::Type CommitType)
{
	ConfigAsset->Config = EditableTextBox->GetText();
}


void SConfigAssetEditor::HandleConfigAssetPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent)
{
	if (Object == ConfigAsset)
	{
		EditableTextBox->SetText(ConfigAsset->Config);
	}
}


#undef LOCTEXT_NAMESPACE
