// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ConfigAssetEditorToolkit.h"

#include "Editor.h"
#include "EditorReimportHandler.h"
#include "EditorStyleSet.h"
#include "SConfigAssetEditor.h"
#include "ConfigAsset.h"
#include "UObject/NameTypes.h"
#include "Widgets/Docking/SDockTab.h"

#define LOCTEXT_NAMESPACE "FConfigAssetEditorToolkit"

DEFINE_LOG_CATEGORY_STATIC(LogConfigAssetEditor, Log, All);


/* Local constants
 *****************************************************************************/

namespace ConfigAssetEditor
{
	static const FName AppIdentifier("ConfigAssetEditorApp");
	static const FName TabId("ConfigEditor");
}


/* FConfigAssetEditorToolkit structors
 *****************************************************************************/

FConfigAssetEditorToolkit::FConfigAssetEditorToolkit(const TSharedRef<ISlateStyle>& InStyle)
	: ConfigAsset(nullptr)
	, Style(InStyle)
{ }


FConfigAssetEditorToolkit::~FConfigAssetEditorToolkit()
{
	FReimportManager::Instance()->OnPreReimport().RemoveAll(this);
	FReimportManager::Instance()->OnPostReimport().RemoveAll(this);

	GEditor->UnregisterForUndo(this);
}


/* FConfigAssetEditorToolkit interface
 *****************************************************************************/

void FConfigAssetEditorToolkit::Initialize(UConfigAsset* InConfigAsset, const EToolkitMode::Type InMode, const TSharedPtr<class IToolkitHost>& InToolkitHost)
{
	ConfigAsset = InConfigAsset;

	// Support undo/redo
	ConfigAsset->SetFlags(RF_Transactional);
	GEditor->RegisterForUndo(this);

	// create tab layout
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("Standalone_ConfigAssetEditor")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
				->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewSplitter()
						->SetOrientation(Orient_Vertical)
						->SetSizeCoefficient(0.66f)
						->Split
						(
							FTabManager::NewStack()
								->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
								->SetHideTabWell(true)
								->SetSizeCoefficient(0.1f)
								
						)
						->Split
						(
							FTabManager::NewStack()
								->AddTab(ConfigAssetEditor::TabId, ETabState::OpenedTab)
								->SetHideTabWell(true)
								->SetSizeCoefficient(0.9f)
						)
				)
		);

	FAssetEditorToolkit::InitAssetEditor(
		InMode,
		InToolkitHost,
		ConfigAssetEditor::AppIdentifier,
		Layout,
		true /*bCreateDefaultStandaloneMenu*/,
		true /*bCreateDefaultToolbar*/,
		InConfigAsset
	);

	RegenerateMenusAndToolbars();
}


/* FAssetEditorToolkit interface
 *****************************************************************************/

FString FConfigAssetEditorToolkit::GetDocumentationLink() const
{
	return FString(TEXT("https://github.com/ue4plugins/ConfigAsset"));
}


void FConfigAssetEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_ConfigAssetEditor", "Config Asset Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(ConfigAssetEditor::TabId, FOnSpawnTab::CreateSP(this, &FConfigAssetEditorToolkit::HandleTabManagerSpawnTab, ConfigAssetEditor::TabId))
		.SetDisplayName(LOCTEXT("ConfigEditorTabName", "Config Editor"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewports"));
}


void FConfigAssetEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(ConfigAssetEditor::TabId);
}


/* IToolkit interface
 *****************************************************************************/

FConfig FConfigAssetEditorToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "Config Asset Editor");
}


FName FConfigAssetEditorToolkit::GetToolkitFName() const
{
	return FName("ConfigAssetEditor");
}


FLinearColor FConfigAssetEditorToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}


FString FConfigAssetEditorToolkit::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "ConfigAsset ").ToString();
}


/* FGCObject interface
 *****************************************************************************/

void FConfigAssetEditorToolkit::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(ConfigAsset);
}


/* FEditorUndoClient interface
*****************************************************************************/

void FConfigAssetEditorToolkit::PostUndo(bool bSuccess)
{ }


void FConfigAssetEditorToolkit::PostRedo(bool bSuccess)
{
	PostUndo(bSuccess);
}


/* FConfigAssetEditorToolkit callbacks
 *****************************************************************************/

TSharedRef<SDockTab> FConfigAssetEditorToolkit::HandleTabManagerSpawnTab(const FSpawnTabArgs& Args, FName TabIdentifier)
{
	TSharedPtr<SWidget> TabWidget = SNullWidget::NullWidget;

	if (TabIdentifier == ConfigAssetEditor::TabId)
	{
		TabWidget = SNew(SConfigAssetEditor, ConfigAsset, Style);
	}

	return SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			TabWidget.ToSharedRef()
		];
}


#undef LOCTEXT_NAMESPACE
