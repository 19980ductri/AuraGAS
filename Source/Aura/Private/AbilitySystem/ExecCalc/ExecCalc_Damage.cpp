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


	//TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
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

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const FGameplayEffectSpec Spec,
	FAggregatorEvaluateParameters EvaluationParam,
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToCaptureDefs) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1);
		if (TypeDamage > -.5f) // .5 padding for floating point [im]precision
		{
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance, false, -1);
			float TargetDebuffResistance = 0;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToCaptureDefs[ResistanceTag],EvaluationParam,TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0);
			const float EffectiveDebuffChance = SourceDebuffChance * (100 - TargetDebuffResistance) / 100.f;
			const bool bDebuff = FMath::RandRange(1,100) <  EffectiveDebuffChance;
			
			if (bDebuff)
			{
				UE_LOG(LogTemp,Warning, TEXT("debuff chance: %f"), EffectiveDebuffChance);
			}
			
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armour, DamageStatic().ArmourDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, DamageStatic().BlockChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmourPenetration, DamageStatic().ArmourPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, DamageStatic().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, DamageStatic().CriticalHitDamageDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, DamageStatic().CriticalHitResistanceDef);
		
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, DamageStatic().FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, DamageStatic().LightningResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, DamageStatic().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, DamageStatic().PhysicalResistanceDef);
	
	

	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}


	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel =  ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}
	
	/*
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);
	*/

	
	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();
	//more like MMC_MaxHealth
	FAggregatorEvaluateParameters EvaluationParam;
	EvaluationParam.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParam.TargetTags = Spec.CapturedSourceTags.GetAggregatedTags();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	DetermineDebuff(ExecutionParams, Spec, EvaluationParam, TagsToCaptureDefs);
	
	
	//Get Damage Set by caller
	float Damage = 0;
	for (const auto& Pair : Tags.DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
			
		checkf(TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesnot contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
				
		const FGameplayEffectAttributeCaptureDefinition CaptureDefinition = TagsToCaptureDefs[ResistanceTag];
		
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
	//UE_LOG(LogTemp,Warning, TEXT("Block chance: %f"), TargetBlockChance);
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
	const float ArmourPenetrationCoefficient = ArmourPenCurve->Eval(SourcePlayerLevel);									

	const float TargetEffectiveArmour = TargetArmour * (100 - SourceArmourPen * ArmourPenetrationCoefficient) /100.f;

	const FRealCurve* EffectiveArmourCurve = ClassInfo->DamageCalculationCoefficient->FindCurve(FName("EffectiveArmour"), FString()); 
	const float EffectiveArmourCoefficient = EffectiveArmourCurve->Eval(TargetPlayerLevel);


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
		const float EffectiveCritCoefficient = EffectiveCritResistCurve->Eval(SourcePlayerLevel);
		const float EffectiveCriticalHitChance = SourceCriticalChance - TargetCriticalResist * EffectiveCritCoefficient;
		const bool bCriticalHit = FMath::RandRange(1,100) < EffectiveCriticalHitChance;

		UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
		Damage = bCriticalHit ? Damage * 2.f + SourceCriticalDamage : Damage;
	}

	//UE_LOG(LogTemp, Warning, TEXT("damage: %f"), Damage);
	const FGameplayModifierEvaluatedData EvaluatedData(DamageStatic().IncomingDamageProperty, EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData); 
}
