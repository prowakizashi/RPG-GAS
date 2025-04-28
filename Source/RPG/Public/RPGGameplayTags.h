#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RPGGameplayTags.generated.h"

/**
 * RPGGameplayTags
 *
 * Singleton containing native gamplay tags
 */
USTRUCT()
struct RPG_API FRPGGameplayTags
{
	GENERATED_BODY()
public:
	static const FRPGGameplayTags& Get();
	static void InitializeNativeGameplayTags();

	/**
	 * Primary Attributes
	 */
	
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	/**
	 * Secondary Attributes
	 */
	
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	
	/**
	 * Vitals Attributes
	 */
	
	FGameplayTag Attributes_Vitals_Health;
	FGameplayTag Attributes_Vitals_Mana;
	
	/**
	 * Resistance Attributes
	 */

	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;
	
	/**
	 * Meta Attributes
	 */

	FGameplayTag Attributes_Meta_IncomingXp;


	/**
	 * Effects
	 */

	FGameplayTag Effects_HitReact;

	/**
	 * Player
	 */
	
	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_CursorTrace;
	
	/**
	 * Damage Types
	 */
	
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
	
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	
	/**
	 * Debuff Types
	 */

	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Arcane;
	FGameplayTag Debuff_Physical;
	
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;
	
	/**
	 * Debuff Attributes
	 */
	
	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Frequency;
	
	/**
	 * Abilities
	 */

	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Lightning_Electrocute;
	
	FGameplayTag Abilities_HitReact;

	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;
	
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;
	
	/**
	 * Cooldowns
	 */
	
	// Todo: Instead of having a cooldown tag for each ability, we could have one for each Input slot (or ability slot)
	// Todo: Then it would mean to have a unique cooldown effect that grant a dynamically assigned slot-cooldown-tag with a dynamically set duration
	FGameplayTag Cooldown_Fire_FireBolt;
	FGameplayTag Cooldown_Lightning_Electrocute;
	
	/**
	 * Combat Socket
	 */
	
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_Tail;
	
	/**
	 * Montage
	 */
	
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	/**
	 * Inputs
	 */
	
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_Spell_1;
	FGameplayTag InputTag_Spell_2;
	FGameplayTag InputTag_Spell_3;
	FGameplayTag InputTag_Spell_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;
	
private:
	
	bool bInitialized = false;
	static FRPGGameplayTags GameplayTags;
	
};
