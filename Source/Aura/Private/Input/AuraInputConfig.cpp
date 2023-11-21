// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for(auto& Action : AbilityInputActions)
	{
		if (Action.InputAction && InputTag == Action.InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for tag [%s] on InputTag [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
