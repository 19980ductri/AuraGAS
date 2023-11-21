// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interact/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayAbility;
UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:

	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	

	/** Combat interface*/
	virtual int32 GetPlayerLevel() override;

	UPROPERTY(EditAnywhere, Category= "Combat")
	FName WeaponTipSocketName;
	
	virtual FVector GetCombatSocketLocation() override;

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

	virtual void Die() override;
	/** end Combat interface*/

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
	
	virtual FVector GetFacingTarget() override;
protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();
	UPROPERTY(EditAnywhere, Category= "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category= "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	
	UPROPERTY(EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect>  DefaultSecondaryAttributes;

	UPROPERTY(EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect>  DefaultVitalAttributes;
	
	
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> AttributesGameplayEffect, float Level) const;
	virtual void InitializeDefaultAttributes()const;

	void AddCharacterAbilities();

	void Dissolve();
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
private:

	UPROPERTY(EditAnywhere, Category= "GAS|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	
	UPROPERTY(EditAnywhere, Category= "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category= "Combat")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	
	UPROPERTY(EditAnywhere, Category= "Combat")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	
};