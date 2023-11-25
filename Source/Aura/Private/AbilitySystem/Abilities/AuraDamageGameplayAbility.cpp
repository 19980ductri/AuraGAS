// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Character/AuraCharacter.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	AActor* SourceActor = GetAbilitySystemComponentFromActorInfo()->GetAvatarActor();
	//bool bHaveSameTag = false;
	for (const FName& SourceTag : SourceActor->Tags)
	{
		for (const FName& TargetTag : TargetActor->Tags)
		{
			if (SourceTag == TargetTag)
			{
				return;
			}
		}
	}
	
	
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1);

	for	(auto Pair : DamageTypes )
	{
		float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UE_LOG(LogTemp, Warning, TEXT("dmg: %f"), ScaledDamage);
		//float health = Cast<UAuraAttributeSet>(Cast<AAuraCharacter>(TargetActor)->GetAttributeSet())->GetHealthAttributeData();
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	
}
