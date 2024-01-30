// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExtendEditor.h"

#include "ContentBrowserModule.h"
#include "DebugHeader.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"
#include "SlateWidgets/AdvanceDeletionWidget.h"
#define LOCTEXT_NAMESPACE "FExtendEditorModule"

void FExtendEditorModule::StartupModule()
{
	InitCBMenuExtention();
	RegisterAdvanceDeletionTab();
}

void FExtendEditorModule::ShutdownModule()
{
}
#pragma region ContentBrowser
void FExtendEditorModule::InitCBMenuExtention()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(
		TEXT("ContentBrowser"));
	//get hold of  all the menu extenders
	TArray<FContentBrowserMenuExtender_SelectedPaths>& ContentBrowserModuleMenuExtender = ContentBrowserModule.
		GetAllPathViewContextMenuExtenders();

	/*FContentBrowserMenuExtender_SelectedPaths CustomCBBMenuDelegate;
	CustomCBBMenuDelegate.BindRaw(this, &FExtendEditorModule::CustomCBMenuExtender);
	ContentBrowserModuleMenuExtender.Add(CustomCBBMenuDelegate);*/
	// add custom delegate to all existing delegates
	ContentBrowserModuleMenuExtender.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(
		this,
		&FExtendEditorModule::CustomCBMenuExtender));
}

// define the position for inserting menu entry
TSharedRef<FExtender> FExtendEditorModule::CustomCBMenuExtender(const TArray<FString>& SelectedPaths)
{
	TSharedRef<FExtender> MenuExtender(new FExtender());
	if (SelectedPaths.Num() > 0)
	{
		MenuExtender->AddMenuExtension(FName("Delete"), //extension hook
		                               EExtensionHook::After, //inserting after
		                               TSharedPtr<FUICommandList>(), //custom host keys
		                               FMenuExtensionDelegate::CreateRaw(this, &FExtendEditorModule::AddCBMenuEntry));
		//second binding will define details of the menu entry
		FolderPathsSelected = SelectedPaths;
	}

	return MenuExtender;
}

void FExtendEditorModule::AddCBMenuEntry(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		FText::FromString("Delete Unused Assets"), //title
		FText::FromString(TEXT("Safely delete all unused asset under folder")), //tool tip
		FSlateIcon(), //icon
		FExecuteAction::CreateRaw(this, &FExtendEditorModule::OnDeleteUnsusedAssetsButtonClicked)
		//the actual function to be executed
	);


	MenuBuilder.AddMenuEntry(
		FText::FromString("Advance Deletion"), //title
		FText::FromString(TEXT("List assets by specific condition in a tab for deleting")), //tool tip
		FSlateIcon(), //icon
		FExecuteAction::CreateRaw(this, &FExtendEditorModule::OnAdvanceDeletionButtonClicked)
		//the actual function to be executed
	);
}

void FExtendEditorModule::OnDeleteUnsusedAssetsButtonClicked()
{
	if (FolderPathsSelected.Num() > 1)
	{
		DebugHeader::ShowMessageDialog(EAppMsgType::Ok,TEXT("You can only do this to one folder"));
		return;
	}
	TArray<FString> AssetPathNames = UEditorAssetLibrary::ListAssets(FolderPathsSelected[0]);
	if (AssetPathNames.Num() > 0)
	{
		EAppReturnType::Type ConfirmResult = DebugHeader::ShowMessageDialog(EAppMsgType::YesNo, TEXT("A total of ")
		                                                                    + FString::FromInt(AssetPathNames.Num())
		                                                                    + TEXT(
			                                                                    " foundn \nWould you like to proceed?"));
		if (ConfirmResult == EAppReturnType::No)
		{
			return;
		}
		TArray<FAssetData> UnusedAssetDatas;

		for (const auto& AssetPathName : AssetPathNames)
		{
			//Dont touch root folder
			if (AssetPathName.Contains(TEXT("Developers"))
				|| AssetPathName.Contains(TEXT("Collections"))
				|| AssetPathName.Contains(TEXT("__ExternalActors__"))
				|| AssetPathName.Contains(TEXT("__ExternalObjects__"))
			)
			{
				continue;
			}
			if (!UEditorAssetLibrary::DoesAssetExist(AssetPathName))
			{
				continue;
			}

			TArray<FString> AssetRefs = UEditorAssetLibrary::FindPackageReferencersForAsset(AssetPathName);
			if (AssetRefs.Num() == 0)
			{
				const FAssetData& UnusedAssetData = UEditorAssetLibrary::FindAssetData(AssetPathName);
				UnusedAssetDatas.Add(UnusedAssetData);
			}
		}

		if (UnusedAssetDatas.Num() > 0)
		{
			ObjectTools::DeleteAssets(UnusedAssetDatas);
		}
	}
}

void FExtendEditorModule::OnAdvanceDeletionButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(FName("AdvanceDeletion"));
}

#pragma endregion

#pragma region CustomEditorTab
void FExtendEditorModule::RegisterAdvanceDeletionTab()
{
	FTabSpawnerEntry& TabSpawnerEntry = FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("AdvanceDeletion"),
		FOnSpawnTab::CreateRaw(this, &FExtendEditorModule::OnSpawnTabAdvanceDeletion));
	TabSpawnerEntry.SetDisplayName(FText::FromString(TEXT("AdvanceDeletion")));
}

TSharedRef<SDockTab> FExtendEditorModule::OnSpawnTabAdvanceDeletion(const FSpawnTabArgs& SpawnTabArgs)
{
	/*TSharedRef<SDockTab> AdvanceDeletionDockTab;
	AdvanceDeletionDockTab = */

	return SNew(SDockTab).TabRole(NomadTab)
	[
		SNew(SAdvanceDeletionTab).AssetDatasToStore(GetAllAssetDatasUnderSelectedFolder())
	];
}

TArray<TSharedPtr<FAssetData>> FExtendEditorModule::GetAllAssetDatasUnderSelectedFolder()
{
	TArray<TSharedPtr<FAssetData>> AvailableAssetData;
	TArray<FString> AssetPathNames = UEditorAssetLibrary::ListAssets(FolderPathsSelected[0]);
	for (const auto& AssetPathName : AssetPathNames)
	{
		//Dont touch root folder
		if (AssetPathName.Contains(TEXT("Developers"))
			|| AssetPathName.Contains(TEXT("Collections"))
			|| AssetPathName.Contains(TEXT("__ExternalActors__"))
			|| AssetPathName.Contains(TEXT("__ExternalObjects__"))
		)
		{
			continue;
		}
		if (!UEditorAssetLibrary::DoesAssetExist(AssetPathName))
		{
			continue;
		}

		const FAssetData Data = UEditorAssetLibrary::FindAssetData(AssetPathName);
		AvailableAssetData.Add(MakeShared<FAssetData>(Data));
	}
	return AvailableAssetData;
}
#pragma endregion
#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FExtendEditorModule, ExtendEditor)
