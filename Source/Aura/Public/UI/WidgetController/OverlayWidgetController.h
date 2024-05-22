// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"


struct FAuraAbilityInfo;
class UAbilityInfo;
//forward declare
class UAuraUserWidget;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()
	FUIWidgetRow(){}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetSignature, FUIWidgetRow, Row);


/*
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangeSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangeSignature, float, NewMaxHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangeSignatue, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangeSignatue, float, NewMaxMana);
*/



UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadCastInitialValue() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category= "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChange;

	
	UPROPERTY(BlueprintAssignable, Category= "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChange;
	
	UPROPERTY(BlueprintAssignable, Category= "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChange;

	UPROPERTY(BlueprintAssignable, Category= "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChange;

	UPROPERTY(BlueprintAssignable, Category= "GAS|Messages")
	FMessageWidgetSignature MessageWidgetRowDelegate;

	
	
	UPROPERTY(BlueprintAssignable, Category="GAS|XP")
	FOnAttributeChangedSignature OnXPPercentChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Level")
	FOnPlayerStatChangedSignature OnPlayerLevelChangedDelegate;
	
protected:
	/*void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void ManaChanged(const FOnAttributeChangeData& Data) const;
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;
	*/
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= WidgetData)
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	//void OnInitializeStartupAbilities(UAuraAbilitySystemComponent* InAuraAbilitySystemComponent);

	void OnXPChanged(int32 NewXP);

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot) const;
};

template<typename T>
T* GetDataTableByTag(UDataTable* Table, const FGameplayTag& Tag)
{
	T* Row = Table->FindRow<T>(Tag.GetTagName(), TEXT(""));
	return Row;

}