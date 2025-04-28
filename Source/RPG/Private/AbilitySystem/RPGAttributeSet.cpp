#include "AbilitySystem/RPGAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "RPGGameplayTags.h"
#include "AbilitySystem/RPGAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Player/RPGPlayerController.h"

URPGAttributeSet::URPGAttributeSet()
{
	const FRPGGameplayTags& RPGTags = FRPGGameplayTags::Get();
	
	/* Primary Attributes */
	TagsToAttributes.Add(RPGTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(RPGTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(RPGTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(RPGTags.Attributes_Primary_Vigor, GetVigorAttribute);
	
	/*  Secondary Attributes */
	TagsToAttributes.Add(RPGTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(RPGTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(RPGTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(RPGTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(RPGTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(RPGTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(RPGTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(RPGTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(RPGTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(RPGTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
	
	/* Vitals Attributes */
	TagsToAttributes.Add(RPGTags.Attributes_Vitals_Health, GetHealthAttribute);
	TagsToAttributes.Add(RPGTags.Attributes_Vitals_Mana, GetManaAttribute);
	
	/* Resistance Attributes */
	TagsToAttributes.Add(RPGTags.Attributes_Resistance_Fire, GetFireResistanceAttribute);
	TagsToAttributes.Add(RPGTags.Attributes_Resistance_Lightning, GetLightningResistanceAttribute);
	TagsToAttributes.Add(RPGTags.Attributes_Resistance_Arcane, GetArcaneResistanceAttribute);
	TagsToAttributes.Add(RPGTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);
}

void URPGAttributeSet::BeginDestroy()
{
	Super::BeginDestroy();
}

void URPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	
	// Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	// Vital Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	// Resistance Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
}

void URPGAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Strength, OldStrength);
}

void URPGAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Intelligence, OldIntelligence);
}

void URPGAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Resilience, OldResilience);
}

void URPGAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Vigor, OldVigor);
}

/**
 * Secondary Attributes
*/

void URPGAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Armor, OldArmor);
}

void URPGAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void URPGAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, BlockChance, OldBlockChance);
}

void URPGAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void URPGAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void URPGAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void URPGAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void URPGAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void URPGAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxHealth, OldMaxHealth);
}

void URPGAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxMana, OldMaxMana);
}

/**
 * Vital Attributes
 */

void URPGAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Health, OldHealth);
}

void URPGAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Mana, OldMana);
}

void URPGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxMana());
	}
}

void URPGAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, FireResistance, OldResistance);
}

void URPGAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, LightningResistance, OldResistance);
}

void URPGAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, ArcaneResistance, OldResistance);
}

void URPGAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, PhysicalResistance, OldResistance);
}

void URPGAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	// Source can be a character (player or mob), or an effect actor. We try to retrieve all the source properties if valid.
	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	// Target can only be a character (player or mob) so we retrieve target properties more easily.
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void URPGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProps;
	SetEffectProperties(Data, EffectProps);

	// check if target is dead
	if (EffectProps.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(EffectProps.TargetCharacter))
	{
		return;
	}
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0, GetMaxMana()));
	}
	
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncommingDamage(EffectProps);
	}
	
	if (Data.EvaluatedData.Attribute == GetIncomingXpAttribute())
	{
		HandleIncommingXp(EffectProps);
	}
}

void URPGAttributeSet::HandleIncommingDamage(const FEffectProperties& EffectProps)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	if (LocalIncomingDamage > 0)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0, GetMaxHealth()));
			
		const bool bFatal = NewHealth <= 0.f;
		if (bFatal)
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProps.TargetCharacter))
			{
				CombatInterface->Die();
			}
			SendXPEvent(EffectProps);
		}
		else
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FRPGGameplayTags::Get().Effects_HitReact);
				
			// apply hit react ability. Ability is found by tag (owning tag)
			EffectProps.TargetASC->TryActivateAbilitiesByTag(TagContainer);
		}
	}

	const bool bBlockedHit = URPGAbilitySystemLibrary::IsBlockedHit(EffectProps.EffectContextHandle);
	const bool bCriticalHit = URPGAbilitySystemLibrary::IsCriticalHit(EffectProps.EffectContextHandle);
	ShowFloatingText(EffectProps, LocalIncomingDamage, bBlockedHit, bCriticalHit);

	if (URPGAbilitySystemLibrary::IsSuccessfulDebuff(EffectProps.EffectContextHandle))
	{
		HandleDebuff(EffectProps);
	}
		
	// we clear the damage
	SetIncomingDamage(0.f);
}

void URPGAttributeSet::HandleDebuff(const FEffectProperties& EffectProps)
{
	FGameplayEffectContextHandle EffectContext = EffectProps.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(EffectProps.SourceAvatarActor);

	const FRPGGameplayTags& RPGTags = FRPGGameplayTags::Get();
	const FGameplayTag DamageType = URPGAbilitySystemLibrary::GetDamageType(EffectProps.EffectContextHandle);
	const float DebuffDamage = URPGAbilitySystemLibrary::GetDebuffDamage(EffectProps.EffectContextHandle);
	const float DebuffDuration = URPGAbilitySystemLibrary::GetDebuffDuration(EffectProps.EffectContextHandle);
	const float DebuffFrequency = URPGAbilitySystemLibrary::GetDebuffFrequency(EffectProps.EffectContextHandle);
	
	// This is how we dynamic create an effect in c++
	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *URPGAbilitySystemLibrary::GetDamageType(EffectProps.EffectContextHandle).ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), *DebuffName);
	
	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);

	FInheritedTagContainer TagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& Component = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	TagContainer.Added.AddTag(RPGTags.DamageTypesToDebuffs[DamageType]);
	Component.SetAndApplyTargetTagChanges(TagContainer);

	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	const int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];

	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = GetIncomingDamageAttribute();

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		FRPGGameplayEffectContext* RPGContext = static_cast<FRPGGameplayEffectContext*>(MutableSpec->GetContext().Get());
		RPGContext->SetDamageType(MakeShareable<FGameplayTag>(new FGameplayTag(DamageType)));

		EffectProps.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}

void URPGAttributeSet::HandleIncommingXp(const FEffectProperties& EffectProps)
{
	const float LocalIncomingXp = GetIncomingXp();
	SetIncomingXp(0.f);

	if (EffectProps.SourceCharacter->Implements<UPlayerInterface>() && EffectProps.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetCharacterLevel(EffectProps.SourceCharacter);
		const int32 CurrentXp = IPlayerInterface::Execute_GetXp(EffectProps.SourceCharacter);
		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(EffectProps.SourceCharacter, CurrentXp + LocalIncomingXp);

		if (CurrentLevel < NewLevel)
		{
			for (int i = CurrentLevel; i < NewLevel; ++i)
			{
				const auto& LevelUpInfo = IPlayerInterface::Execute_GetLevelUpInfo(EffectProps.SourceCharacter, i);
				IPlayerInterface::Execute_LevelUp(EffectProps.SourceCharacter, 1, LevelUpInfo.AttributePointReward, LevelUpInfo.SpellPointReward);
			}
				
			bTopOffHealth = true;
			bTopOffMana = true;
		}
			
		IPlayerInterface::Execute_AddToXp(EffectProps.SourceCharacter, LocalIncomingXp);
	}
}

void URPGAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		bTopOffHealth = false;
		SetHealth(GetMaxHealth());
	}
	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		bTopOffMana = false;
		SetMana(GetMaxMana());
	}
	
}

void URPGAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit)
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		if (ARPGPlayerController* PC = Cast<ARPGPlayerController>(Props.SourceController))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
		}
		else if (ARPGPlayerController* PC2 = Cast<ARPGPlayerController>(Props.TargetController))
		{
			PC2->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
		}
	}
}

void URPGAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 XPReward = URPGAbilitySystemLibrary::GetXpRewardForClassAndReward(
			Props.TargetCharacter,
			ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter),
			ICombatInterface::Execute_GetCharacterLevel(Props.TargetCharacter));
		FGameplayEventData Payload;
		Payload.EventTag = FRPGGameplayTags::Get().Attributes_Meta_IncomingXp;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, Payload.EventTag, Payload);
	}
}
