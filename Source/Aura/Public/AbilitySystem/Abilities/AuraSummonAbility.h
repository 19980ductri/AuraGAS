// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure) 
	TSubclassOf<APawn> GetRandomMinionClass();
	
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocation();

	UPROPERTY(EditAnywhere, Category= "Summoning")
	int32 NumMinions;

	UPROPERTY(EditDefaultsOnly, Category= "Summoing")
	TArray<TSubclassOf<APawn>> MinionClasses;
	
	UPROPERTY(EditDefaultsOnly, Category= "Summoing")
	float MinSpawnDistance = 50;

	UPROPERTY(EditDefaultsOnly, Category= "Summoing")
	float MaxSpawnDistance = 250;

	UPROPERTY(EditDefaultsOnly, Category= "Summoing")
	float SpawnSpread = 90;

	
};

