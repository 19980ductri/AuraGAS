// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraAbilityTypes.h"
#include "Interact/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"
 
/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

protected:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Damage")
	FScalableFloat Damage;*/
	/*
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
	*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDuration = 5.f;
		
public:
	UFUNCTION(BlueprintCallable, Category= "Cause Damage")
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure, Category = "Attack|montage")
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages);

	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;
	//float GetDamageByDamageType(float InLevel, const FGameplayTag& DamageTypeTag);
};
