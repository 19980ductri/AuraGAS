// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interact/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UCameraComponent;
class UNiagaraComponent;

UCLASS(Blueprintable,BlueprintType)
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

#pragma region PlayerInterface
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToPlayerSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual void AddTooPlayerAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual int32 GetAttributePoints_Implementation() override;
	virtual int32 GetSpellPoints_Implementation() override;
#pragma endregion 
	
	virtual int32 GetPlayerLevel_Implementation() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagraComponent;
private:
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> TopDownCam;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	/*UPROPERTY(EditAnywhere, Category= "GAS|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;*/
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles();	
};