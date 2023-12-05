// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "GameFramework/Character.h"
#include "Interact/CombatInterface.h"


struct AuraDamageStatics
{
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(IncomingDamage)
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armour); //macro for declaring attribute captures
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmourPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)


	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
	AuraDamageStatics()
	{

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, IncomingDamage, Source, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armour, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmourPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);

		const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armour, ArmourDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, BlockChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef);
		
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, ArcaneResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, PhysicalResistanceDef);
	}
};

static const AuraDamageStatics& DamageStatic()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}


UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatic().ArmourDef);
	RelevantAttributesToCapture.Add(DamageStatic().IncomingDamageDef);
	RelevantAttributesToCapture.Add(DamageStatic().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().ArmourPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatic().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().CriticalHitDamageDef);

	RelevantAttributesToCapture.Add(DamageStatic().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().PhysicalResistanceDef);
	
		
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	
	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();
	//more like MMC_MaxHealth
	FAggregatorEvaluateParameters EvaluationParam;
	EvaluationParam.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParam.TargetTags = Spec.CapturedSourceTags.GetAggregatedTags();


	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	
	//Get Damage Set by caller
	float Damage = 0;

	
	for (const auto& Pair : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
			
		checkf(AuraDamageStatics().TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesnot contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
				
		const FGameplayEffectAttributeCaptureDefinition CaptureDefinition = AuraDamageStatics().TagsToCaptureDefs[ResistanceTag];
		
		float Resistance = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition, EvaluationParam, Resistance);
		Resistance = FMath::Clamp(Resistance, 0, 100);
		
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false);
		
		DamageTypeValue *= (100.f - Resistance) / 100;
		
		Damage += DamageTypeValue;
	}
	
	float TargetBlockChance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().BlockChanceDef, EvaluationParam, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0);
	UE_LOG(LogTemp,Warning, TEXT("Block chance: %f"), TargetBlockChance);
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;	
	
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	
	if (bBlocked) Damage = Damage / 2.f;
	
	//Armour and Armour penetrate
	float TargetArmour = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().ArmourDef, EvaluationParam, TargetArmour);
	TargetArmour = FMath::Max<float>(TargetArmour, 0);
	
	float SourceArmourPen = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().ArmourPenetrationDef, EvaluationParam, SourceArmourPen);
	SourceArmourPen = FMath::Max<float>(SourceArmourPen,0 );

	const UCharacterClassInfo* ClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);

	const FRealCurve* ArmourPenCurve = ClassInfo->DamageCalculationCoefficient->FindCurve(FName("ArmourPenetration"), FString());
	const float ArmourPenetrationCoefficient = ArmourPenCurve->Eval(SourceCombatInterface->GetPlayerLevel());									

	const float TargetEffectiveArmour = TargetArmour * (100 - SourceArmourPen * ArmourPenetrationCoefficient) /100.f;

	const FRealCurve* EffectiveArmourCurve = ClassInfo->DamageCalculationCoefficient->FindCurve(FName("EffectiveArmour"), FString()); 
	const float EffectiveArmourCoefficient = EffectiveArmourCurve->Eval(TargetCombatInterface->GetPlayerLevel());


	Damage *= (100 - TargetEffectiveArmour * EffectiveArmourCoefficient) / 100.f;

	if (SourceAvatar->Tags.Contains(FName("Player")))
	{
		//Critical Hit and Crit resist
		float SourceCriticalChance = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().CriticalHitChanceDef, EvaluationParam, SourceCriticalChance);
		SourceCriticalChance = FMath::Max<float>(SourceCriticalChance, 0);
	
		float SourceCriticalDamage = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().CriticalHitDamageDef, EvaluationParam, SourceCriticalDamage);
		SourceCriticalDamage  = FMath::Max<float>(SourceCriticalDamage, 0);
	
		float TargetCriticalResist = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().CriticalHitResistanceDef, EvaluationParam, TargetCriticalResist);
		TargetCriticalResist = FMath::Max<float>(TargetCriticalResist, 0);

		const FRealCurve* EffectiveCritResistCurve = ClassInfo->DamageCalculationCoefficient->FindCurve(FName("CriticalHitResistance"), FString());
		const float EffectiveCritCoefficient = EffectiveCritResistCurve->Eval(SourceCombatInterface->GetPlayerLevel());
		const float EffectiveCriticalHitChance = SourceCriticalChance - TargetCriticalResist * EffectiveCritCoefficient;
		const bool bCriticalHit = FMath::RandRange(1,100) < EffectiveCriticalHitChance;

		UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
		Damage = bCriticalHit ? Damage * 2.f + SourceCriticalDamage : Damage;
	}

	UE_LOG(LogTemp, Warning, TEXT("damage: %f"), Damage);
	const FGameplayModifierEvaluatedData EvaluatedData(DamageStatic().IncomingDamageProperty, EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData); 
}
