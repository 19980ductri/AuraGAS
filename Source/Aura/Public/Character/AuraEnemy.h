// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interact/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UAnimMontage;
class UBehaviorTree;
class AAuraAIController;

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual int32 GetPlayerLevel_Implementation() override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintReadOnly, Category= "React")
	bool bHitReacting = false;
	void HitReactTagChanged(const FGameplayTag CallBackTag, int32 NewCount);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseWalkSpeed = 150;

	UPROPERTY(EditAnywhere, Category= "Combat", BlueprintReadOnly)
	float LifeSpan = 5;
	virtual void Die() override;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

	UPROPERTY(BlueprintReadWrite, Category= "Combat")
	TObjectPtr<AActor> CombatTarget;
	
	//virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void InitAbilityActorInfo() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void InitializeDefaultAttributes() const override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Character Class Defaults")
	int32 Level = 1;

	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	
private:

	void ChangeHealth(const FOnAttributeChangeData& Data);
	void ChangeMaxHealth(const FOnAttributeChangeData& Data);
};
