#include "AbilitySystem/MMC/MMC_MaxHealth.h"

#include "AbilitySystem/RPGAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = URPGAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	// List of Captured attributes that we will use in the calculation
	// Then we will be able to retrieve their value with GetCapturedAttributeMagnitude()
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target (in order to identify them ?)
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Set the aggregated tags
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.f);

	int32 CharacterLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		CharacterLevel = ICombatInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	}

	return 80.f + Vigor * 2.5f + 10.f * CharacterLevel;
}
