#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RPGAbilityTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UExecCalc_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:
	
	void DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FRPGGameplayEffectContext* EffectContext, const FAggregatorEvaluateParameters& EvaluationParameters) const;
	void ApplyResistances(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluationParameters, float& OutDamage) const;
	void DetermineBlockHit(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FRPGGameplayEffectContext* EffectContext, const FAggregatorEvaluateParameters& EvaluationParameters, float& OutDamage) const;
	void ApplyArmor(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluationParameters, UCharacterClassInfo* CharacterClassInfo, int32 SourceLevel, int32 TargetLevel, float& OutDamage) const;
	void DetermineCriticalHit(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FRPGGameplayEffectContext* EffectContext, const FAggregatorEvaluateParameters& EvaluationParameters, UCharacterClassInfo* CharacterClassInfo, int32 TargetLevel, float& OutDamage) const;
};
