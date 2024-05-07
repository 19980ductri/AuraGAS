// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

struct FGameplayEffectModCallbackData;

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()
	
	FEffectProperties(){}
	
	FGameplayEffectContextHandle EffectContextHandle;
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	UPROPERTY()
	AController* SourceController = nullptr;
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY()
	AController* TargetController = nullptr;
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;

};

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPointer;
//typedef FGameplayAttribute(* FAttributeFuncPointer)(); //-> this is the same as above
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;
/*
 * typedef is specific to FGameplayAttribute() signatur, but TStaticFuncPtr is generic to any signature chosen
 */
// no need
//CLARE_DELEGATE_RetVal(FGameplayAttribute, FAttributeSignature);

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit);

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	//TMap<FGameplayTag, FAttributeSignature> TagsToAttributes;

	/*
	use this map instead 
	TMap<FGameplayTag, TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr> TagsToAttributes;
	 */
	 //this below is reduced version of the declaration for a function pointer above
	
	//TMap<FGameplayTag, FGameplayAttribute(*)()> TagsToAttributes; 
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes; 
	/* function potion that: take no input parameter, return FGameplayAttribute
	TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FunctionPointer;*/

	/*
	 * primary attributes
	 */

	//physical damage
	UPROPERTY(BlueprintReadOnly, Category= "Primary" ,ReplicatedUsing= OnRep_Strength)
	FGameplayAttributeData Strength; 
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	//magical damage
	UPROPERTY(BlueprintReadOnly, Category= "Primary" ,ReplicatedUsing= OnRep_Intelligence)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	//attack effector
	UPROPERTY(BlueprintReadOnly, Category= "Primary" ,ReplicatedUsing= OnRep_Agility)
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Agility);
	UFUNCTION()
	void OnRep_Agility(const FGameplayAttributeData& OldAgility) const;
	
	//armour
	UPROPERTY(BlueprintReadOnly, Category= "Primary" ,ReplicatedUsing= OnRep_Resilience)
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	//health
	UPROPERTY(BlueprintReadOnly, Category= "Primary" ,ReplicatedUsing= OnRep_Vigor)
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;
	
	/*
	 * primary attributes
	 */
	
	/*****************************************************************************************************************************/

	/*
	 * vital attributes
	 */
	
	UPROPERTY(BlueprintReadOnly, Category= "Vital" ,ReplicatedUsing= OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	
	
	UPROPERTY(BlueprintReadOnly, Category= "Vital" ,ReplicatedUsing= OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	
	
	/*
	 * vital attributes
	 */

	/*
	 * Second Attributes
	 */

	UPROPERTY(BlueprintReadOnly, Category= "Second", ReplicatedUsing= OnRep_Armour)
	FGameplayAttributeData Armour;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armour);
	UFUNCTION()
	void OnRep_Armour(const FGameplayAttributeData& OldArmour) const;

	UPROPERTY(BlueprintReadOnly, Category= "Second", ReplicatedUsing= OnRep_ArmourPenetration)
	FGameplayAttributeData ArmourPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmourPenetration);
	UFUNCTION()
	void OnRep_ArmourPenetration(const FGameplayAttributeData& OldArmourPenetration) const;
	

	UPROPERTY(BlueprintReadOnly, Category= "Second", ReplicatedUsing= OnRep_BlockChance)
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	UPROPERTY(BlueprintReadOnly, Category= "Second", ReplicatedUsing= OnRep_CriticalHitChance)
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance);
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	UPROPERTY(BlueprintReadOnly, Category= "Second", ReplicatedUsing= OnRep_CriticalHitDamage)
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage);
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

	UPROPERTY(BlueprintReadOnly, Category= "Second", ReplicatedUsing= OnRep_CriticalHitResistance)
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance);
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;

	UPROPERTY(BlueprintReadOnly, Category= "Second", ReplicatedUsing= OnRep_HealthRegen)
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegen);
	UFUNCTION()
	void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const;

	UPROPERTY(BlueprintReadOnly, Category= "Second", ReplicatedUsing= OnRep_ManaRegen)
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegen);
	UFUNCTION()
	void OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen) const;

	UPROPERTY(BlueprintReadOnly, Category= "Second", ReplicatedUsing= OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UPROPERTY(BlueprintReadOnly, Category= "Second" ,ReplicatedUsing= OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHeath) const;
	

	UPROPERTY(BlueprintReadOnly, Category= "Second" ,ReplicatedUsing= OnRep_FireDamage)
	FGameplayAttributeData FireDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireDamage);
	UFUNCTION()
	void OnRep_FireDamage(const FGameplayAttributeData& OldFireDamage) const;
	
	
	UPROPERTY(BlueprintReadOnly, Category= "Second" ,ReplicatedUsing= OnRep_FireResistance)
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireResistance);
	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;
	
	
	UPROPERTY(BlueprintReadOnly, Category= "Second" ,ReplicatedUsing= OnRep_LightningDamage)
	FGameplayAttributeData LightningDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningDamage);
	UFUNCTION()
	void OnRep_LightningDamage(const FGameplayAttributeData& OldLightningDamage) const;
	
	
	UPROPERTY(BlueprintReadOnly, Category= "Second" ,ReplicatedUsing= OnRep_LightningResistance)
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningResistance);
	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const;
	

	UPROPERTY(BlueprintReadOnly, Category= "Second" ,ReplicatedUsing= OnRep_ArcaneDamage)
	FGameplayAttributeData ArcaneDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArcaneDamage);
	UFUNCTION()
	void OnRep_ArcaneDamage(const FGameplayAttributeData& OldArcaneDamage) const;
	
	
	UPROPERTY(BlueprintReadOnly, Category= "Second" ,ReplicatedUsing= OnRep_ArcaneResistance)
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArcaneResistance);
	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const;
	

	UPROPERTY(BlueprintReadOnly, Category= "Second" ,ReplicatedUsing= OnRep_PhysicalDamage)
	FGameplayAttributeData PhysicalDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalDamage);
	UFUNCTION()
	void OnRep_PhysicalDamage(const FGameplayAttributeData& OldPhysicalDamage) const;
	
	
	UPROPERTY(BlueprintReadOnly, Category= "Second" ,ReplicatedUsing= OnRep_PhysicalResistance)
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalResistance);
	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;
	
	/*
	 * Meta attributes
	 */
	UPROPERTY(BlueprintReadOnly, Category= "Meta")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category= "Meta")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingXP);
	
private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data,FEffectProperties& Props) const;

	void SendXPEvent(const FEffectProperties& Props);


	bool bTopOffHealth = false;
	bool bTopOffMana = false;
};


