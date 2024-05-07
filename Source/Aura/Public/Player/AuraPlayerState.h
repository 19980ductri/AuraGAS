// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*value*/)

class UAbilitySystemComponent;
class UAttributeSet;
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:

	AAuraPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;
	
	int32 GetPlayerLevel() const;
	void AddToLevel(int32 InLevel);
	void SetLevel(int32 InLevel);
	
	int32 GetXP() const;
	void SetXP(int32 NewXP);
	void AddToXP(int32 XPAdded);

	void SetAttributePoint(int32 InAttributePoint);
	void AddToAttributePoint(int32 InAttributePoint);
	int32 GetAttributePoint() const;

	void SetSpellPoint(int32 InSpellPoint);
	void AddToSpellPoint(int32 InSpellPoint);
	int32 GetSpellPoint() const;
	
	
	FOnPlayerStatChanged OnXPChangedDelegate;
	FOnPlayerStatChanged OnLevelChangedDelegate;
	FOnPlayerStatChanged OnAttributePointChangedDelegate;
	FOnPlayerStatChanged OnSpellPointChangedDelegate;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeSet> AttributeSet;
		
private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP;
	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoint)
	int32 AttributePoint = 0;
	UFUNCTION()
	void OnRep_AttributePoint();

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SpellPoint)
	int32 SpellPoint = 0;
	UFUNCTION()
	void OnRep_SpellPoint();
};
