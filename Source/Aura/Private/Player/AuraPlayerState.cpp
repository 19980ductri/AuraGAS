// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100;
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	/*
	 * for mixed Replication mode: the controller must be the owner of OwnerActor. For Pawns, this is set auto in PossessedBy()
	 * the owner of PlayerState is auto set is the Controller
	 * if OwnerActor is not the PlayerState, SetOwner() must be called on the OwnerActor to set its owner to the Controller
	 */
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
	DOREPLIFETIME(AAuraPlayerState, AttributePoint);
	DOREPLIFETIME(AAuraPlayerState, SpellPoint);
}


UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

int32 AAuraPlayerState::GetPlayerLevel() const
{
	return Level;
}

void AAuraPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

int32 AAuraPlayerState::GetXP() const
{
	return  XP;
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::OnRep_AttributePoint()
{
	OnAttributePointChangedDelegate.Broadcast(AttributePoint);
}

void AAuraPlayerState::OnRep_SpellPoint()
{
	OnSpellPointChangedDelegate.Broadcast(SpellPoint);
}

void AAuraPlayerState::SetXP(int32 NewXP)
{
	XP = NewXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::AddToXP(int32 XPAdded)
{
	XP += XPAdded;
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::SetAttributePoint(int32 InAttributePoint)
{
	AttributePoint = InAttributePoint;
	OnAttributePointChangedDelegate.Broadcast(AttributePoint);
}

void AAuraPlayerState::AddToAttributePoint(int32 InAttributePoint)
{
	AttributePoint += InAttributePoint;
	OnAttributePointChangedDelegate.Broadcast(AttributePoint);
}

int32 AAuraPlayerState::GetAttributePoint() const
{
	return AttributePoint;
}


void AAuraPlayerState::SetSpellPoint(int32 InSpellPoint)
{
	SpellPoint = InSpellPoint;
	OnSpellPointChangedDelegate.Broadcast(SpellPoint);
}

void AAuraPlayerState::AddToSpellPoint(int32 InSpellPoint)
{
	SpellPoint += InSpellPoint;
	OnSpellPointChangedDelegate.Broadcast(SpellPoint);
}


int32 AAuraPlayerState::GetSpellPoint() const
{
	return SpellPoint;
}
