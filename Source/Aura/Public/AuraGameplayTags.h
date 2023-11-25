// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"


/**
 * Singleton contain native gameplaytags
 *
 */
struct FAuraGameplayTags
{
public:
 static const FAuraGameplayTags& Get() {return GameplayTags; }
 static void InitializeNativeGameplayTags();

 //vital
 FGameplayTag Attributes_Vital_Health;
 FGameplayTag Attributes_Vital_Mana;

 //primary
 FGameplayTag Attributes_Primary_Strength;
 FGameplayTag Attributes_Primary_Intelligence;
 FGameplayTag Attributes_Primary_Agility;
 FGameplayTag Attributes_Primary_Resilience;
 FGameplayTag Attributes_Primary_Vigor;
 
 //Secondary
 FGameplayTag Attributes_Secondary_MaxMana;
 FGameplayTag Attributes_Secondary_MaxHealth;
 FGameplayTag Attributes_Secondary_Armour;
 FGameplayTag Attributes_Secondary_ArmourPenetration;
 FGameplayTag Attributes_Secondary_BlockChance;
 FGameplayTag Attributes_Secondary_CriticalHitChance;
 FGameplayTag Attributes_Secondary_CriticalHitDamage;
 FGameplayTag Attributes_Secondary_CriticalHitResistance;
 FGameplayTag Attributes_Secondary_HealthRegen;
 FGameplayTag Attributes_Secondary_ManaRegen;

 FGameplayTag Attributes_Resistance_Fire;
 FGameplayTag Attributes_Resistance_Lightning;
 FGameplayTag Attributes_Resistance_Arcane;
 FGameplayTag Attributes_Resistance_Physical;

 //Input
 FGameplayTag InputTag_LMB;
 FGameplayTag InputTag_RMB;
 FGameplayTag InputTag_1;
 FGameplayTag InputTag_2;
 FGameplayTag InputTag_3;
 FGameplayTag InputTag_4;

 //damge
 FGameplayTag Damage;

 FGameplayTag Damage_Fire;
 FGameplayTag Damage_Lightning;
 FGameplayTag Damage_Arcane;
 FGameplayTag Damage_Physical;

 FGameplayTag Abilities_Attack;
 FGameplayTag Montage_Attack_Weapon;
 FGameplayTag Montage_Attack_LeftHand;
 FGameplayTag Montage_Attack_RightHand;

 
 TMap<FGameplayTag,FGameplayTag> DamageTypesToResistances;
 
 // effects
 FGameplayTag Effects_HitReact;


protected:

private:
 static FAuraGameplayTags GameplayTags;
 
};
