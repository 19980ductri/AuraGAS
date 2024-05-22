// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannels.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParam)
{
	PlayerController = WCParam.PlayerController;
	PlayerState = WCParam.PlayerState;
	AbilitySystemComponent = WCParam.AbilitySystemComponent;
	AttributeSet = WCParam.Attributes;
}

void UAuraWidgetController::BroadCastInitialValue()
{
	
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
	
}

AAuraPlayerController* UAuraWidgetController::GetAuraPC()
{
	if (AuraPlayerController == nullptr)
	{
		AuraPlayerController = CastChecked<AAuraPlayerController>(PlayerController);
	}
	return AuraPlayerController;
}

AAuraPlayerState* UAuraWidgetController::GetAuraPS()
{
	if (AuraPlayerState == nullptr)
	{
		AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	}
	return AuraPlayerState;
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	}
	return AuraAbilitySystemComponent;
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAS()
{
	if (AuraAttributeSet == nullptr)
	{
		AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	}
	return AuraAttributeSet;
}

void UAuraWidgetController::BroadCastAbilityInfo()
{
	//UE_LOG(LogAura, Warning, TEXT("Menu that BroadCastAbilityInfo: %s"), *this->GetName());
	if (!GetAuraASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = AuraAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetAuraASC()->ForEachAbility(BroadcastDelegate);
}
