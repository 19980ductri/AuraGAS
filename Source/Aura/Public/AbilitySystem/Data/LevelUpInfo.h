// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)//,Category="LevelUp")
	int32 LevelUpRequirement = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 AttributePointReward = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SpellPointReward = 0;
};


UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraLevelUpInfo> LevelUpInformations;

	int32 FindLevelForXP(int32 XP) const;
	
};
