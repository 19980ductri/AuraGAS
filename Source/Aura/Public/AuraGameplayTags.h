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
 FGameplayTag InputTag_Passive_1;
 FGameplayTag InputTag_Passive_2;
 

 //damge
 FGameplayTag Damage;

 FGameplayTag Damage_Fire;
 FGameplayTag Damage_Lightning;
 FGameplayTag Damage_Arcane;
 FGameplayTag Damage_Physical;

 FGameplayTag Abilities_None;
 
 FGameplayTag Abilities_Attack;
 FGameplayTag Abilities_Summon;

 FGameplayTag Abilities_HitReact;

 FGameplayTag Abilities_Status_Locked;
 FGameplayTag Abilities_Status_Unlocked;
 FGameplayTag Abilities_Status_Eligible;
 FGameplayTag Abilities_Status_Equipped;

 FGameplayTag Abilities_Type_Offensive;
 FGameplayTag Abilities_Type_Passive;
 FGameplayTag Abilities_Type_None;
 
 FGameplayTag Abilities_Fire_FireBolt; 
 FGameplayTag Cooldown_Fire_FireBolt;

 FGameplayTag Abilities_Lightning_Electrocute;
 FGameplayTag Cooldown_Lightning_Electrocute;

 
 FGameplayTag CombatSocket_Weapon;
 FGameplayTag CombatSocket_LeftHand;
 FGameplayTag CombatSocket_RightHand;
 FGameplayTag CombatSocket_Tail;

 FGameplayTag Montage_Attack_1;
 FGameplayTag Montage_Attack_2;
 FGameplayTag Montage_Attack_3;
 FGameplayTag Montage_Attack_4;


 //debuff
 FGameplayTag Debuff_Burn;
 FGameplayTag Debuff_Stun;
 FGameplayTag Debuff_Arcane;
 FGameplayTag Debuff_Physical;

 FGameplayTag Debuff_Chance;
 FGameplayTag Debuff_Damage;
 FGameplayTag Debuff_Duration;
 FGameplayTag Debuff_Frequency;
 
 
 // effects
 FGameplayTag Effects_HitReact;

 // meta
 FGameplayTag Attribute_Meta_IncomingXP;

 // key = damage type, value = resistance type;
 TMap<FGameplayTag,FGameplayTag> DamageTypesToResistances;

 // key = damage type, value = debuff type
 TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;


 
protected:

private:
 static FAuraGameplayTags GameplayTags;
 
};
