// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API  UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Lib| Widget Controller")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldObject);

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Lib| Widget Controller")
	static UAttributeMenuWidgetController* GetAttributeWidgetController(const UObject* WorldObject);

	
	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Lib| Widget Controller")
	static void InitializeDefaultAttributes(const UObject* WorldObject, ECharacterClass CharacterClass, float level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Lib| Widget Controller")
	static void GiveStartupAbilites(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION()
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextContext);

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Lib | Gameplay Effects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Lib | Gameplay Effects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Lib | Gameplay Effects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Lib | Gameplay Effects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category= "Gameplay mechanic")
	static void GetLivePlayerWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
		const TArray<AActor*>& ActorsToIgnore,float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);
};

