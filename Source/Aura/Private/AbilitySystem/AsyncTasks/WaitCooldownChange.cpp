// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"
#include "Aura/AuraLogChannels.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCoolDownChange(UAbilitySystemComponent* AbilitySystemComponent,
                                                                const FGameplayTag& InCooldownTag)
{

	//UE_LOG(LogAura, Warning, TEXT("sfasdf"));
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}
	
	//To know when a cooldown has ended (Cooldown Tag has been removed)
	AbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTag,
		EGameplayTagEventType::NewOrRemoved).AddUObject(
			WaitCooldownChange,
			&UWaitCooldownChange::CoolDownTagChange);

	//To know when a cooldown effect applied
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &ThisClass::OnActiveEffectAdded);
	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (IsValid(ASC) == false)
	{
		return;
	}
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange::CoolDownTagChange(const FGameplayTag InCoolDownTag, int32 NewCount)
{
	if (NewCount <= 0)
	{
		CooldownEnd.Broadcast(0);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC,
	const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaning	= ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);

		if (TimesRemaning.Num() > 0)
		{
			float TimeRemaining = TimesRemaning[0];
			for (int32 i = 0; i < TimesRemaning.Num(); i++)
			{
				if (TimesRemaning[i] > TimeRemaining)
				{
					TimeRemaining = TimesRemaning[i];
				}
			}
			CooldownStart.Broadcast(TimeRemaining);
		}

		
	}
	
}
