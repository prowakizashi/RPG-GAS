#include "AbilitySystem/MMC/MMC_MaxMana.h"

#include "AbilitySystem/RPGAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = URPGAttributeSet::GetVigorAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	// List of Captured attributes that we will use in the calculation
	// Then we will be able to retrieve their value with GetCapturedAttributeMagnitude()
	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Intelligence = 0;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);
	Intelligence = FMath::Max<float>(Intelligence, 0.f);

	int32 CharacterLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		CharacterLevel = ICombatInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	}

	return 50.f + Intelligence * 2.5f + 15.f * CharacterLevel;
}
