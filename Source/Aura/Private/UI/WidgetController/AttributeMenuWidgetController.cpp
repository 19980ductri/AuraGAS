 // Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"


 void UAttributeMenuWidgetController::BroadCastInitialValue()
{
	Super::BroadCastInitialValue();
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);
	
	//broadcast from AuraAttributeSet
	//so in the future if we plan to add more attributes to characters,
	//we don't need to come here and do anything, just go to AuraAttributeSet and add them to the TMap
	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

 	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
 	AttributePointsChangedDelegate.Broadcast(AuraPlayerState->GetAttributePoint());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
 	AuraPlayerState->OnAttributePointChangedDelegate.AddLambda(
 		[this](int32 Points)
 		{
 			AttributePointsChangedDelegate.Broadcast(Points);
 		}
 	);
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair, AS](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		});
	}
}

 void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
 {
 	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
 	AuraASC->UpgradeAttribute(AttributeTag);
 }

 void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                             const FGameplayAttribute& Attribute)
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
