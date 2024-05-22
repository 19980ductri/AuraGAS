#pragma once

#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.Generated.h"


USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()
	FDamageEffectParams(){}
	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceASC;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC;
	UPROPERTY()
	float BaseDamage = 0;

	UPROPERTY()
	float AbilityLevel = 0;

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY()
	float DebuffChance = 0;

	UPROPERTY()
	float DebuffDamage = 0;
	UPROPERTY()
	float DebuffDuration = 0;

	UPROPERTY()
	float DebuffFrequency = 0;
};


USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}

	/* Creates a copy of this context, used to  duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			NewContext->AddHitResult(*GetHitResult(), true);
			
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
private:


protected:
	bool bIsCriticalHit = false;

	bool bIsBlockedHit = false;

public:
	
	FORCEINLINE	bool IsCriticalHit() const{ return bIsCriticalHit;}
	FORCEINLINE bool IsBlockedHit() const {return bIsBlockedHit;}

	void SetIsCriticalHit(const bool IsCritical) { bIsCriticalHit = IsCritical; }
	void SetIsBlockedHit(const bool IsBlocked) {bIsBlockedHit = IsBlocked;}

};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
