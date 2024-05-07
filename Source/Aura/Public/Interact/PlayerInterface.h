// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class AURA_API IPlayerInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 InXP);
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 Level);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(int32 Level);

	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InPlayerLevel);

	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerSpellPoints(int32 InSpellPoints);

	UFUNCTION(BlueprintNativeEvent)
	void AddTooPlayerAttributePoints(int32 InAttributePoints);
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints();
};
