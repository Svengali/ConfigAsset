// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ConfigAssetActions.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "ConfigAsset.h"
#include "Styling/SlateStyle.h"

#include "ConfigAssetEditorToolkit.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions"


/* FConfigAssetActions constructors
 *****************************************************************************/

FConfigAssetActions::FConfigAssetActions(const TSharedRef<ISlateStyle>& InStyle)
	: Style(InStyle)
{ }


/* FAssetTypeActions_Base overrides
 *****************************************************************************/

bool FConfigAssetActions::CanFilter()
{
	return true;
}


void FConfigAssetActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	FAssetTypeActions_Base::GetActions(InObjects, MenuBuilder);

	auto ConfigAssets = GetTypedWeakObjectPtrs<UConfigAsset>(InObjects);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ConfigAsset_ReverseConfig", "Reverse Config"),
		LOCTEXT("ConfigAsset_ReverseConfigToolTip", "Reverse the text stored in the selected text asset(s)."),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateLambda([=]{
				for (auto& ConfigAsset : ConfigAssets)
				{
					if (ConfigAsset.IsValid() && !ConfigAsset->Config.IsEmpty())
					{
						ConfigAsset->Config = FText::FromString(ConfigAsset->Config.ToString().Reverse());
						ConfigAsset->PostEditChange();
						ConfigAsset->MarkPackageDirty();
					}
				}
			}),
			FCanExecuteAction::CreateLambda([=] {
				for (auto& ConfigAsset : ConfigAssets)
				{
					if (ConfigAsset.IsValid() && !ConfigAsset->Config.IsEmpty())
					{
						return true;
					}
				}
				return false;
			})
		)
	);
}


uint32 FConfigAssetActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}


FText FConfigAssetActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_ConfigAsset", "Config Asset");
}


UClass* FConfigAssetActions::GetSupportedClass() const
{
	return UConfigAsset::StaticClass();
}


FColor FConfigAssetActions::GetTypeColor() const
{
	return FColor::White;
}


bool FConfigAssetActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return true;
}


void FConfigAssetActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid()
		? EToolkitMode::WorldCentric
		: EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		auto ConfigAsset = Cast<UConfigAsset>(*ObjIt);

		if (ConfigAsset != nullptr)
		{
			TSharedRef<FConfigAssetEditorToolkit> EditorToolkit = MakeShareable(new FConfigAssetEditorToolkit(Style));
			EditorToolkit->Initialize(ConfigAsset, Mode, EditWithinLevelEditor);
		}
	}
}


#undef LOCTEXT_NAMESPACE
