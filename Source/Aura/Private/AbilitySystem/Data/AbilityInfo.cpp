// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound)
{

	for (const FAuraAbilityInfo& Info : AbilityInformations)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		//UE_LOG(LogTemp, )
		UE_LOG(LogAura, Error, TEXT("Cannot find info for Ability TAG [%s] on AbilityInfo[%s]"), *AbilityTag.ToString(), *GetNameSafe(this));

	}
	return FAuraAbilityInfo();
}
