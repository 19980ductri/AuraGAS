// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAbilityInfo;
class AAuraHUD;
struct FWidgetControllerParams;
class USpellMenuWidgetController;
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

	
	UFUNCTION(BlueprintPure, Category= "Aura Ability System Lib| Widget Controller", meta = (DefaultToSelf = "WorldObject"))
	static bool MakeWidgetControllerParams (const UObject* WorldObject, FWidgetControllerParams& OutWCParam, AAuraHUD*& OutAuraHUD );
	
	UFUNCTION(BlueprintPure, Category= "Aura Ability System Lib| Widget Controller", meta = (DefaultToSelf = "WorldObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldObject);

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Lib| Widget Controller", meta = (DefaultToSelf = "WorldObject"))
	static UAttributeMenuWidgetController* GetAttributeWidgetController(const UObject* WorldObject);

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Lib| Widget Controller", meta = (DefaultToSelf = "WorldObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldObject);
		
	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Lib|  Character Class Default")
	static void InitializeDefaultAttributes(const UObject* WorldObject, ECharacterClass CharacterClass, float level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Lib|  Character Class Default")
	static void GiveStartupAbilites(const UObject* WorldObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Lib| Character Class Default")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextContext);

	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Lib| Character Class Default")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

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

	static int32 GetXPRewardForClassAndLevel(const UObject* WorldObject, ECharacterClass CharacterClass, int32 CharacterLevel);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);
	
};

