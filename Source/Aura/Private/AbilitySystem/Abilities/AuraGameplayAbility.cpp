// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - Loremasdfjhjkhjkhaskjdhfklj", Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level, </><Level>%d</> \n<Default>Cause much more damage, </> "), Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default> Spell Locked Until Level </><Level>%d"), Level);
}

float UAuraGameplayAbility::GetManaCost(float InLevel)
{
	float ManaCost = 0;
	if (const UGameplayEffect* CostEffect =  GetCostGameplayEffect())
	{
		for (auto Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				return ManaCost;
			}
		}
	}
	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(float InLevel)
{
	float Cooldown = 0;
	if (const UGameplayEffect* CoolDownEffect = GetCooldownGameplayEffect())
	{
		CoolDownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}
	return Cooldown;
}
