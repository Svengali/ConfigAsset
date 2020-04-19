// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SConfigAssetEditor.h"

#include "Fonts/SlateFontInfo.h"
#include "Internationalization/Config.h"
#include "ConfigAsset.h"
#include "UObject/Class.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SMultiLineEditableConfigBox.h"

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
				SAssignNew(EditableConfigBox, SMultiLineEditableConfigBox)
					.BackgroundColor((Settings != nullptr) ? Settings->BackgroundColor : FLinearColor::White)
					.Font((Settings != nullptr) ? Settings->Font : FSlateFontInfo())
					.ForegroundColor((Settings != nullptr) ? Settings->ForegroundColor : FLinearColor::Black)
					.Margin((Settings != nullptr) ? Settings->Margin : 4.0f)
					.OnConfigChanged(this, &SConfigAssetEditor::HandleEditableConfigBoxConfigChanged)
					.OnConfigCommitted(this, &SConfigAssetEditor::HandleEditableConfigBoxConfigCommitted)
					.Config(ConfigAsset->Config)
			]
	];

	FCoreUObjectDelegates::OnObjectPropertyChanged.AddSP(this, &SConfigAssetEditor::HandleConfigAssetPropertyChanged);
}


/* SConfigAssetEditor callbacks
 *****************************************************************************/

void SConfigAssetEditor::HandleEditableConfigBoxConfigChanged(const FConfig& NewConfig)
{
	ConfigAsset->MarkPackageDirty();
}


void SConfigAssetEditor::HandleEditableConfigBoxConfigCommitted(const FConfig& Comment, EConfigCommit::Type CommitType)
{
	ConfigAsset->Config = EditableConfigBox->GetConfig();
}


void SConfigAssetEditor::HandleConfigAssetPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent)
{
	if (Object == ConfigAsset)
	{
		EditableConfigBox->SetConfig(ConfigAsset->Config);
	}
}


#undef LOCTEXT_NAMESPACE
