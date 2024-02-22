// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FExtendEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
private:

#pragma region ContentBrowser
	void InitCBMenuExtention();

	TArray<FString> FolderPathsSelected;
	
	TSharedRef<FExtender> CustomCBMenuExtender(const TArray<FString>& SelectedPaths);
	
	void AddCBMenuEntry( class FMenuBuilder& MenuBuilder);

	void OnDeleteUnsusedAssetsButtonClicked();
	
	void OnAdvanceDeletionButtonClicked();
#pragma endregion 

#pragma region CustomEditorTab

	void RegisterAdvanceDeletionTab();
	TSharedRef<SDockTab> OnSpawnTabAdvanceDeletion(const FSpawnTabArgs& SpawnTabArgs);

	TArray<TSharedPtr<FAssetData>> GetAllAssetDatasUnderSelectedFolder();
	
#pragma endregion 

	
};
