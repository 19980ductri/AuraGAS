// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EEffectApplyPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};
UENUM(BlueprintType)
enum class EEffectRemovalPolicy
{
	RemoveOnOverlap,
	RemoveOnEndOverlap,
	DoNotRemove
};
class UGameplayEffect;


UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

	
private:
	void ApplyEffectOnOverlap(AActor* TargetActor);
	void ApplyEffectOnEndOverlap(AActor* TargetActor);
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable)
	void EndOverlap(AActor* TargetActor);
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	bool bDestroyOnEffectApplication;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	bool bApplyEffectsToEnemies = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	EEffectApplyPolicy InstantEffectyPolicy = EEffectApplyPolicy::DoNotApply;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	EEffectApplyPolicy DurationEffectPolicy = EEffectApplyPolicy::DoNotApply	;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	EEffectApplyPolicy InfiniteEffectApplyPolicy = EEffectApplyPolicy::DoNotApply;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::DoNotRemove;

	
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	float ActorLevel = 1;
	
};
