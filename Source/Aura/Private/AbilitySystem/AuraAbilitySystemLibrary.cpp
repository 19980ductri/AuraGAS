// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AuraAbilityTypes.h"
#include "Game/AuraGameModeBase.h"
#include "Interact/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();
			const FWidgetControllerParams Params = FWidgetControllerParams(PlayerController,PlayerState ,AbilitySystemComponent, AttributeSet);
			
			return AuraHUD->GetOverlayWidgetController(Params);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeWidgetController(const UObject* WorldObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldObject, 0))
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

void UAuraAbilitySystemLibrary::GiveStartupAbilites(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
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
		ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor());
		if (CombatInterface)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextContext)
{
	if (const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextContext)))
	{
		return AuraGameMode->CharacterClassInfo;
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