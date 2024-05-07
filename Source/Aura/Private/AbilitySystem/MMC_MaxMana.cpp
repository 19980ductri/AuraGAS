// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interact/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(IntelligenceDef);
	
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParam;
	EvaluationParam.SourceTags = SourceTags;
	EvaluationParam.TargetTags = TargetTags;

	float Intelligence = 0;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParam, Intelligence);
	Intelligence = FMath::Max(Intelligence,0);

	int32 PlayerLevel = 1;

	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}
	/*
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
	*/

	return 100 + 3 * Intelligence + 11.5 * PlayerLevel;
}
