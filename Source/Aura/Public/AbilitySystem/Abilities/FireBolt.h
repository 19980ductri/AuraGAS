// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "FireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()

	
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	
};
