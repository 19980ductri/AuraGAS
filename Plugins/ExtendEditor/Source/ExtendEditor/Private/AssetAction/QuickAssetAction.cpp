// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetAction/QuickAssetAction.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"

void UQuickAssetAction::DuplicateAsset(int32 NumOfDuplicates)
{
	if (NumOfDuplicates <= 0)
	{
		DebugHeader::ShowMessageDialog(EAppMsgType::Ok, TEXT("Please enter valid number"));
		return;
	}
	TArray<FAssetData> SelectedAssetDatas = UEditorUtilityLibrary::GetSelectedAssetData();

	uint32 Counter = 0;
	for (const FAssetData& AssetData : SelectedAssetDatas)
	{
		for (int i = 0; i < NumOfDuplicates; i++)
		{
			const FString SourceAssetPath = AssetData.GetObjectPathString();
			const FString NewDuplcatedAssetName = AssetData.AssetName.ToString() + TEXT("_") + FString::FromInt(i + 1);
			const FString NewPathName = FPaths::Combine(AssetData.PackagePath.ToString(), NewDuplcatedAssetName);

			if (UEditorAssetLibrary::DuplicateAsset(SourceAssetPath, NewPathName))
			{
				UEditorAssetLibrary::SaveAsset(NewPathName, false);
				++Counter;
			}
		}
	}

	if (Counter > 0)
	{
		//Print(TEXT("Succesfully duplicated" + FString::FromInt(Counter) + " files"), FColor::Green);
		DebugHeader::ShowNotifyInfo(TEXT("Succesfully duplicated" + FString::FromInt(Counter) + " files"));
	}
}

void UQuickAssetAction::AddPrefixes()
{
	TArray<UObject*> SelectedOjects = UEditorUtilityLibrary::GetSelectedAssets();

	uint32 Counter = 0;
	for (auto SelectedObj : SelectedOjects)
	{
		if (!SelectedObj)
		{
			continue;
		}
		FString* PrefixFound = PrefixMap.Find(SelectedObj->GetClass());
		if (!PrefixFound || PrefixFound->IsEmpty())
		{
			DebugHeader::Print(TEXT("Failed to find prefix for class") + SelectedObj->GetClass()->GetName(), FColor::Red);
			continue;
		}

		FString OldName = SelectedObj->GetName();
		if (OldName.StartsWith(*PrefixFound))
		{
			DebugHeader::Print(OldName + TEXT(" already have prefix added"), FColor::Red);
		}

		if (SelectedObj->IsA<UMaterialInstance>())
		{
			OldName.RemoveFromStart(TEXT("M_"));
			OldName.RemoveFromEnd(TEXT("_Inst"));
		}

		const FString NewNameWithPrefix = *PrefixFound + OldName;

		UEditorUtilityLibrary::RenameAsset(SelectedObj, NewNameWithPrefix);
		++Counter;
	}

	if (Counter <= 0)
	{
		return;
	}
	DebugHeader::ShowNotifyInfo(TEXT("Successfully renamed ") + FString::FromInt(Counter) + " asset");
}

void UQuickAssetAction::RemoveUnusedAssets()
{
	TArray<FAssetData> SelectedAssetDatas = UEditorUtilityLibrary::GetSelectedAssetData();
	TArray<FAssetData> UnusedAssetDatas;

	for (const auto& SelectedAsset : SelectedAssetDatas)
	{
		TArray<FString> AssetRefs = UEditorAssetLibrary::FindPackageReferencersForAsset(
			SelectedAsset.GetObjectPathString());
		if (AssetRefs.Num() == 0)
		{
			UnusedAssetDatas.Add(SelectedAsset);
		}
	}
	if (UnusedAssetDatas.Num() == 0)
	{
		DebugHeader::ShowMessageDialog(EAppMsgType::Ok,TEXT("No unused asset found"));
		return;
	}

	const int32 NumOfAssetsDeleted = ObjectTools::DeleteAssets(UnusedAssetDatas);
	if (NumOfAssetsDeleted == 0)
	{
		return;
	}
	DebugHeader::ShowMessageDialog(EAppMsgType::Ok,TEXT("Unused assets successfullly deleted"));
}
