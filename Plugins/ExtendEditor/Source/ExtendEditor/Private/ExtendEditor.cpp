// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExtendEditor.h"

#include "ContentBrowserModule.h"
#include "DebugHeader.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"

#define LOCTEXT_NAMESPACE "FExtendEditorModule"

void FExtendEditorModule::StartupModule()
{
	InitCBMenuExtention();
}

void FExtendEditorModule::ShutdownModule()
{
	
}
#pragma region ContentBrowser
void FExtendEditorModule::InitCBMenuExtention()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	
	TArray<FContentBrowserMenuExtender_SelectedPaths>& ContentBrowserModuleMenuExtender = ContentBrowserModule.GetAllPathViewContextMenuExtenders();

	/*FContentBrowserMenuExtender_SelectedPaths CustomCBBMenuDelegate;
	CustomCBBMenuDelegate.BindRaw(this, &FExtendEditorModule::CustomCBMenuExtender);
	ContentBrowserModuleMenuExtender.Add(CustomCBBMenuDelegate);*/

	ContentBrowserModuleMenuExtender.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this, &FExtendEditorModule::CustomCBMenuExtender));
} 

TSharedRef<FExtender> FExtendEditorModule::CustomCBMenuExtender(const TArray<FString>& SelectedPaths)
{
	TSharedRef<FExtender> MenuExtender (new FExtender());
	if (SelectedPaths.Num() > 0)
	{
		MenuExtender->AddMenuExtension(FName("Delete"),
			EExtensionHook::After,
			TSharedPtr<FUICommandList>(),
			FMenuExtensionDelegate::CreateRaw(this, &FExtendEditorModule::AddCBMenuEntry));
		FolderPathsSelected = SelectedPaths;
	}

	return MenuExtender;
}

void FExtendEditorModule::AddCBMenuEntry(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(FText::FromString("Delete Unused Assets"),
		FText::FromString(TEXT("Safely delete all unused asset under folder")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FExtendEditorModule::OnDeleteUnsusedAssetsButtonClicked)
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
				+ TEXT(" foundn \nWould you like to proceed?"));
		if (ConfirmResult == EAppReturnType::No)
		{
			return;
		}
		TArray<FAssetData> UnusedAssetDatas;

		for (const auto& AssetPathName : AssetPathNames)
		{
			//Dont touch root folder
			if (AssetPathName.Contains(TEXT("Developers")) || AssetPathName.Contains(TEXT("Collections")))
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
		
		if (UnusedAssetDatas.Num() > 0 )
		{
			ObjectTools::DeleteAssets(UnusedAssetDatas);
		}
		
	}
}

#pragma endregion 
#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FExtendEditorModule, ExtendEditor)