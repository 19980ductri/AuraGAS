// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "GameplayTagsModule.h"
#include "AuraAbilityTypes.h"
#include "Game/AuraGameModeBase.h"
#include "Interact/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"

bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldObject, 0))
	{
		OutAuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD());
		if (OutAuraHUD)
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();
			OutWCParams.Attributes = AttributeSet;
			OutWCParams.PlayerController = PlayerController;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerState = PlayerState;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldObject)
{
	FWidgetControllerParams WCParam;
	AAuraHUD* AuraHUD;
	bool bSuccessParam = MakeWidgetControllerParams(WorldObject,WCParam, AuraHUD);
	if (bSuccessParam)
	{
		return AuraHUD->GetOverlayWidgetController(WCParam);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeWidgetController(const UObject* WorldObject)
{

	FWidgetControllerParams WCParam;
	AAuraHUD* AuraHUD;
	bool bSuccessParam = MakeWidgetControllerParams(WorldObject,WCParam, AuraHUD);
	if (bSuccessParam)
	{
		return AuraHUD->GetAttributeMenuWidgetController(WCParam);
	}
	return nullptr;
	/*if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();
			const FWidgetControllerParams Params = FWidgetControllerParams(PlayerController,PlayerState ,AbilitySystemComponent, AttributeSet);
			
			return AuraHUD->GetAttributeMenuWidgetController(Params);
		}
	}
	return nullptr;*/
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldObject)
{
	FWidgetControllerParams WCParam;
	AAuraHUD* AuraHUD;
	bool bSuccessParam = MakeWidgetControllerParams(WorldObject,WCParam, AuraHUD);
	if (bSuccessParam)
	{
		return AuraHUD->GetSpellMenuWidgetController(WCParam);
	}
	/*
	return nullptr;
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldObject, 0))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PS = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PS->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PS->GetAttributeSet();
			const FWidgetControllerParams Params = FWidgetControllerParams(PlayerController,PS,AbilitySystemComponent, AttributeSet);
			return HUD->GetSpellMenuWidgetController(Params);
		}
	}
	return nullptr;*/
	return nullptr;
}


void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldObject, ECharacterClass CharacterClass, float level, UAbilitySystemComponent* ASC)
{
	const AActor* AvatarActor = ASC->GetAvatarActor();
	
	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldObject);
	if (ClassInfo == nullptr) return;
	

	const FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->GetCharacterClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryContextHandle = ASC->MakeEffectContext();
	SecondaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes, level, SecondaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalContextHandle =  ASC->MakeEffectContext();
	VitalContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes, level, VitalContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilites(const UObject* WorldObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldObject);
	if (CharacterClassInfo == nullptr)
	{
		return;
	}
	for (auto AbilityClass : CharacterClassInfo->Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	for (auto AbilityClass : DefaultInfo.StartupAbilities)
	{
		
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}


int32 UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldObject);
	if (CharacterClassInfo == nullptr)
	{
		return 0;
	}
	FCharacterClassDefaultInfo Info = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);
	return static_cast<int32>(XPReward);
}

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	const AActor*  SourceAvatarActor = DamageEffectParams.SourceASC->GetAvatarActor();
	FGameplayEffectContextHandle EffectContextHandle =  DamageEffectParams.SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceASC->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, DamageEffectParams.DebuffDamage);

	DamageEffectParams.TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}


UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextContext)
{
	if (const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextContext)))
	{
		return AuraGameMode->CharacterClassInfo;
	}
	return nullptr;
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	if (const AAuraGameModeBase* GameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return GameModeBase->AbilityInfo;
	}
	return nullptr;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraEffectContext)
	{
		return AuraEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraEffectContext)
	{
		return AuraEffectContext->IsCriticalHit();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraEffectContext)
	{
		AuraEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraEffectContext)
	{
		AuraEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}	

void UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParam;
	SphereParam.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;

	const UWorld* World = GEngine-> GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World)
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParam);
		for (auto Overlap : Overlaps)
		{
			AActor* Actor = Overlap.GetActor();
			if (const bool ImplementCombatInterface = Actor->Implements<UCombatInterface>())
			{
				if (const bool IsAlive = !ICombatInterface::Execute_IsDead(Actor))
				{
					OutOverlappingActors.AddUnique(Actor);
				}
			}
		}
	}
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}
