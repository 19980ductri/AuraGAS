// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemGlobal.h"

#include "AuraAbilityTypes.h"

FGameplayEffectContext* UAuraAbilitySystemGlobal::AllocGameplayEffectContext() const
{
	
	return new FAuraGameplayEffectContext();
}
