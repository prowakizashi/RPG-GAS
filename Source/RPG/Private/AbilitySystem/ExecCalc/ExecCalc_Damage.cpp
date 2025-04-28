#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include <rapidjson/schema.h>

#include "AbilitySystemComponent.h"
#include "RPGAbilityTypes.h"
#include "RPGGameplayTags.h"
#include "AbilitySystem/RPGAbilitySystemLibrary.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

struct DamageStatics
{
	// Declare many capture variables for many attribute.
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor); // will be named "ArmorDef"
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
	DamageStatics()
	{
		// Define the capture variables for capturing the named attribute member variable (Armor, etc) of the
		// class "URPGAttributeSet" from the Target (or the Source) of the effect, not snapshoted.
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, BlockChance, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, PhysicalResistance, Target, false);

		// Associate a tag to a capture variable
		const auto& Tags = FRPGGameplayTags::Get();
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, ArmorDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, BlockChanceDef);
		
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, ArcaneResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, PhysicalResistanceDef);
	}

	
};

static const DamageStatics& GetDamageStatics()
{
	static DamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	// Add capture variables defined in the struct DamageStatics
	// It's an alternative way of what we do in MMC_MaxHealth constructor
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().BlockChanceDef);
	
	RelevantAttributesToCapture.Add(GetDamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// TODO: all of this could be stored in 1 data struct to reduce parameters in "apply" and "determine" functions
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourceLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourceLevel = ICombatInterface::Execute_GetCharacterLevel(SourceAvatar);
	}
	
	int32 TargetLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetLevel = ICombatInterface::Execute_GetCharacterLevel(TargetAvatar);
	}

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	FRPGGameplayEffectContext* EffectContext = static_cast<FRPGGameplayEffectContext*>(EffectSpec.GetContext().Get());
	
	const FGameplayTagContainer* SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	UCharacterClassInfo* CharacterClassInfo = URPGAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	check(CharacterClassInfo);

	DetermineDebuff(ExecutionParams, EffectContext, EvaluationParameters);
	
	float Damage = 0.f;
	ApplyResistances(ExecutionParams, EvaluationParameters, Damage);
	DetermineBlockHit(ExecutionParams, EffectContext, EvaluationParameters, Damage);
	ApplyArmor(ExecutionParams, EvaluationParameters, CharacterClassInfo, SourceLevel, TargetLevel, Damage);
	DetermineCriticalHit(ExecutionParams, EffectContext, EvaluationParameters, CharacterClassInfo, TargetLevel, Damage);
	
	const FGameplayModifierEvaluatedData EvaluatedData(URPGAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FRPGGameplayEffectContext* EffectContext, const FAggregatorEvaluateParameters& EvaluationParameters) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	const FRPGGameplayTags& RPGTags = FRPGGameplayTags::Get();
	
	for (const auto& Pair : RPGTags.DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		const float TypeDamage = EffectSpec.GetSetByCallerMagnitude(DamageType, false,-1.f);
		if (TypeDamage >= 0.f)
		{
			const float SourceDebuffChance = EffectSpec.GetSetByCallerMagnitude(RPGTags.Debuff_Chance, false,-1.f);
			const FGameplayTag& ResistanceTag = RPGTags.DamageTypesToResistances[DamageType];
			
			float TargetDebuffResistance = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TagsToCaptureDefs[ResistanceTag], EvaluationParameters, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max(TargetDebuffResistance, 0.0f);

			const float EffectiveDebuffChance = SourceDebuffChance * (100.f - TargetDebuffResistance) / 100.f;
			if (FMath::RandRange(0, 100) < EffectiveDebuffChance)
			{
				FGameplayEffectContextHandle ContextHandle = EffectSpec.GetContext();

				EffectContext->SetIsSuccessfulDebuff(true);
				EffectContext->SetDamageType(MakeShared<FGameplayTag>(DamageType));

				const float DebuffDamage = EffectSpec.GetSetByCallerMagnitude(RPGTags.Debuff_Damage, false,-1.f);
				const float DebuffDuration = EffectSpec.GetSetByCallerMagnitude(RPGTags.Debuff_Duration, false,-1.f);
				const float DebuffFrequency = EffectSpec.GetSetByCallerMagnitude(RPGTags.Debuff_Frequency, false,-1.f);
				
				EffectContext->SetDebuffDamage(DebuffDamage);
				EffectContext->SetDebuffDuration(DebuffDuration);
				EffectContext->SetDebuffFrequency(DebuffFrequency);
			}
		}
	}
}

void UExecCalc_Damage::ApplyResistances(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluationParameters, float& OutDamage) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	const FRPGGameplayTags& RPGTags = FRPGGameplayTags::Get();
	
	for (const auto& Pair : RPGTags.DamageTypesToResistances)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& ResistanceType = Pair.Value;
		
		float DamageTypeValue = EffectSpec.GetSetByCallerMagnitude(DamageType, false);
		if (DamageTypeValue > 0.f)
		{
			checkf(GetDamageStatics().TagsToCaptureDefs.Contains(ResistanceType), TEXT("TagsToCaptureDefs doesn't contain Tag [%s] in ExecCalc_Damage"), *ResistanceType.ToString())
			const auto CaptureDef = GetDamageStatics().TagsToCaptureDefs[ResistanceType];
		
			float TargetResistance = 0.0f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, TargetResistance);
			TargetResistance = FMath::Clamp(TargetResistance, 0.0f, 100.f);
			DamageTypeValue *= (100.f - TargetResistance) / 100.f;
			OutDamage += DamageTypeValue;
		}
	}
}

void UExecCalc_Damage::DetermineBlockHit(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FRPGGameplayEffectContext* EffectContext, const FAggregatorEvaluateParameters& EvaluationParameters, float& OutDamage) const
{
	// Capture BlockChance on Target, and determine if there was a successful Block
	float TargetBlockChance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);

	// If block, half the Damage
	const bool bHasBlocked = FMath::FRandRange(0.f, 100.f) < TargetBlockChance;
	OutDamage = bHasBlocked ? OutDamage / 2.f : OutDamage;
	EffectContext->SetIsBlockedHit(bHasBlocked);
}

void UExecCalc_Damage::ApplyArmor(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluationParameters, UCharacterClassInfo* CharacterClassInfo, int32 SourceLevel, int32 TargetLevel, float& OutDamage) const
{
	float SourceArmor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorDef, EvaluationParameters, SourceArmor);
	SourceArmor = FMath::Max(0.0f, SourceArmor);

	// ArmorPenetration ignores a percentage of the Target's Armor
	float SourceArmorPenetration = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(SourceArmorPenetration, 0.0f);

	float TargetArmor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max(0.0f, TargetArmor);

	// TODO: I don't like how it works. Should be base on difference of level between target and source.
	// TODO: Currently curves have no sense. The higher level you have, the less Penetration and Armor are scaled (so you have less of each)
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceLevel);
	const FRealCurve* ArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("Armor"), FString());
	const float ArmorCoefficient = ArmorCurve->Eval(TargetLevel);

	// Armor calculation to apply to damages
	const float EffectiveArmor = FMath::Max(TargetArmor * (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f, 0.f);
	// Armor reduce damages by percentage
	OutDamage *= (100.f - EffectiveArmor * ArmorCoefficient) / 100.f;
}

void UExecCalc_Damage::DetermineCriticalHit(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FRPGGameplayEffectContext* EffectContext, const FAggregatorEvaluateParameters& EvaluationParameters, UCharacterClassInfo* CharacterClassInfo, int32 TargetLevel, float& OutDamage) const
{
	float SourceCriticalHitChance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max(SourceCriticalHitChance, 0.0f);
	
	float TargetCriticalHitResistance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max(TargetCriticalHitResistance, 0.0f);

	const FRealCurve* CriticalResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalResistance"), FString());
	const float CriticalResistanceCoefficient = CriticalResistanceCurve->Eval(TargetLevel);
	
	const float EffectiveCriticChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalResistanceCoefficient;
	const bool bIsCriticHit = FMath::FRandRange(0.f, 100.f) < EffectiveCriticChance;
	EffectContext->SetIsCriticalHit(bIsCriticHit);

	if (bIsCriticHit)
	{
		float SourceCriticalHitDamage = 0.0f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
		SourceCriticalHitDamage = FMath::Max(SourceCriticalHitDamage, 0.0f);
		OutDamage = OutDamage * 2 + SourceCriticalHitDamage;
	}
}
