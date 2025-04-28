#include "RPGGameplayTags.h"
#include "GameplayTagsManager.h"

FRPGGameplayTags FRPGGameplayTags::GameplayTags;

const FRPGGameplayTags& FRPGGameplayTags::Get()
{
	InitializeNativeGameplayTags();
	return GameplayTags;
}

void FRPGGameplayTags::InitializeNativeGameplayTags()
{
	if (GameplayTags.bInitialized) return;
	
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	/**
	 * Primary Attributes
	 */
	
	GameplayTags.Attributes_Primary_Strength = Manager.AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases physical damages.")
		);
	
	GameplayTags.Attributes_Primary_Intelligence = Manager.AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increases magical damages.")
		);
	
	GameplayTags.Attributes_Primary_Resilience = Manager.AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("Increases Armor and Armor Penetration.")
		);
	
	GameplayTags.Attributes_Primary_Vigor = Manager.AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increases Max Health.")
		);

	/**
	 * Secondary Attributes
	 */
	
	GameplayTags.Attributes_Secondary_Armor = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken, improves Block Chance.")
		);
	
	GameplayTags.Attributes_Secondary_ArmorPenetration = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString("Ignored percentage of enemy Armor, increases Critical Hit Chance.")
		);
	
	GameplayTags.Attributes_Secondary_BlockChance = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("Chance to cut incoming damage in half.")
		);
	
	GameplayTags.Attributes_Secondary_CriticalHitChance = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("Chance to double damage plus critical hit bonus.")
		);
	
	GameplayTags.Attributes_Secondary_CriticalHitDamage = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Bonus damage added when a critical hi is scored.")
		);
	
	GameplayTags.Attributes_Secondary_CriticalHitResistance = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString("Reduce Critical Hit Chance of the attacking enemy.")
		);
	
	GameplayTags.Attributes_Secondary_HealthRegeneration = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("Amount of Health regenerated every 1 second.")
		);
	
	GameplayTags.Attributes_Secondary_ManaRegeneration = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("Amount of Manage regenerated every 1 second.")
		);
	
	GameplayTags.Attributes_Secondary_MaxHealth = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum of Health obtainable.")
		);
	
	GameplayTags.Attributes_Secondary_MaxMana = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("Maximum of Mana obtainable.")
		);
	
	/**
	 * Resistance Attributes
	*/
	
	GameplayTags.Attributes_Resistance_Fire = Manager.AddNativeGameplayTag(
		FName("Attributes.Resistance.Fire"),
		FString("Resistance to Fire damages")
		);
	
	GameplayTags.Attributes_Resistance_Lightning = Manager.AddNativeGameplayTag(
		FName("Attributes.Resistance.Lightning"),
		FString("Resistance to Lightning damages")
		);
	
	GameplayTags.Attributes_Resistance_Arcane = Manager.AddNativeGameplayTag(
		FName("Attributes.Resistance.Arcane"),
		FString("Resistance to Arcane damages")
		);
	
	GameplayTags.Attributes_Resistance_Physical = Manager.AddNativeGameplayTag(
		FName("Attributes.Resistance.Physical"),
		FString("Resistance to Physical damages")
		);
	
	/**
	 * Meta Attributes
	*/
	
	GameplayTags.Attributes_Meta_IncomingXp = Manager.AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingXp"),
		FString("Incoming Xp Meta Attribute")
		);
	
	/**
	 * Damage Types
	*/
	
	GameplayTags.Damage = Manager.AddNativeGameplayTag(
		FName("Damage"),
		FString("Damages")
		);
	
	GameplayTags.Damage_Fire = Manager.AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Fire Damage Type")
		);
	
	GameplayTags.Damage_Lightning = Manager.AddNativeGameplayTag(
		FName("Damage.Lightning"),
		FString("Lightning Damage Type")
		);
	
	GameplayTags.Damage_Arcane = Manager.AddNativeGameplayTag(
		FName("Damage.Arcane"),
		FString("Arcane Damage Type")
		);
	
	GameplayTags.Damage_Physical = Manager.AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage Type")
		);
	
	/**
	 * Map of Damage Types to Resistances
	*/
	
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
	
	
	/**
	 * Debuff Types
	*/
	
	GameplayTags.Debuff_Arcane = Manager.AddNativeGameplayTag(
		FName("Debuff.Arcane"),
		FString("Debuff for Arcane damage")
		);
	
	GameplayTags.Debuff_Burn = Manager.AddNativeGameplayTag(
		FName("Debuff.Burn"),
		FString("Debuff for Fire damage")
		);
	
	GameplayTags.Debuff_Physical = Manager.AddNativeGameplayTag(
		FName("Debuff.Physical"),
		FString("Debuff for Physical damage")
		);
	
	GameplayTags.Debuff_Stun = Manager.AddNativeGameplayTag(
		FName("Debuff.Stun"),
		FString("Debuff for Lightning damage")
		);
	
	/**
	 * Map of Damage Types to Debuffs
	*/
	
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Arcane, GameplayTags.Debuff_Arcane);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_Burn);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Physical, GameplayTags.Debuff_Physical);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Lightning, GameplayTags.Debuff_Stun);

	/**
	 * Debuff Attributes
	*/
	
	GameplayTags.Debuff_Chance = Manager.AddNativeGameplayTag(
		FName("Debuff.Chance"),
		FString("Debuff Chance")
		);
	
	GameplayTags.Debuff_Damage = Manager.AddNativeGameplayTag(
		FName("Debuff.Damage"),
		FString("Debuff Damage")
		);
	
	GameplayTags.Debuff_Duration = Manager.AddNativeGameplayTag(
		FName("Debuff.Duration"),
		FString("Debuff Duration")
		);
	
	GameplayTags.Debuff_Frequency = Manager.AddNativeGameplayTag(
		FName("Debuff.Frequency"),
		FString("Debuff Frequency")
		);
	
	/**
	 * Abilities
	*/
	
	GameplayTags.Abilities_Attack = Manager.AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Attack Ability tag")
		);
	
	GameplayTags.Abilities_Summon = Manager.AddNativeGameplayTag(
		FName("Abilities.Summon"),
		FString("Summon Ability tag")
		);
	
	GameplayTags.Abilities_Fire_FireBolt = Manager.AddNativeGameplayTag(
		FName("Abilities.Fire.FireBolt"),
		FString("FireBolt Ability tag")
		);
	
	GameplayTags.Abilities_Lightning_Electrocute = Manager.AddNativeGameplayTag(
		FName("Abilities.Lightning.Electrocute"),
		FString("Electrocute Ability tag")
		);
	
	GameplayTags.Abilities_HitReact = Manager.AddNativeGameplayTag(
		FName("Abilities.HitReact"),
		FString("Hit React Ability Tag")
		);
	
	GameplayTags.Abilities_Status_Locked = Manager.AddNativeGameplayTag(
		FName("Abilities.Status.Locked"),
		FString("Locked Status")
		);
	
	GameplayTags.Abilities_Status_Eligible = Manager.AddNativeGameplayTag(
		FName("Abilities.Status.Eligible"),
		FString("Eligible Status")
		);
	
	GameplayTags.Abilities_Status_Unlocked = Manager.AddNativeGameplayTag(
		FName("Abilities.Status.Unlocked"),
		FString("Unlocked Status")
		);
	
	GameplayTags.Abilities_Status_Equipped = Manager.AddNativeGameplayTag(
		FName("Abilities.Status.Equipped"),
		FString("Equipped Status")
		);
	
	GameplayTags.Abilities_Type_Offensive = Manager.AddNativeGameplayTag(
		FName("Abilities.Type.Offensive"),
		FString("Offensive Type")
		);
	
	GameplayTags.Abilities_Type_Passive = Manager.AddNativeGameplayTag(
		FName("Abilities.Type.Passive"),
		FString("Passive Type")
		);
	
	GameplayTags.Abilities_Type_None = Manager.AddNativeGameplayTag(
		FName("Abilities.Type.None"),
		FString("None Type")
		);
	
	/**
	 * Cooldowns
	*/
	
	GameplayTags.Cooldown_Fire_FireBolt = Manager.AddNativeGameplayTag(
		FName("Cooldown.Fire.FireBolt"),
		FString("FireBolt Cooldown tag")
		);
	
	GameplayTags.Cooldown_Lightning_Electrocute = Manager.AddNativeGameplayTag(
		FName("Cooldown.Lightning.Electrocute"),
		FString("Electrocute Cooldown tag")
		);

	
	/**
	 * Effects
	*/
	
	GameplayTags.Effects_HitReact = Manager.AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Hit React")
		);
	
	/**
	 * Combat Socket
	*/
	
	GameplayTags.CombatSocket_Weapon = Manager.AddNativeGameplayTag(
		FName("CombatSocket.Weapon"),
		FString("Combat Socket for Weapon.")
		);
	
	GameplayTags.CombatSocket_RightHand = Manager.AddNativeGameplayTag(
		FName("CombatSocket.RightHand"),
		FString("Combat Socket for Right Hand.")
		);
	
	GameplayTags.CombatSocket_LeftHand = Manager.AddNativeGameplayTag(
		FName("CombatSocket.LeftHand"),
		FString("Combat Socket for Left Hand.")
		);
	
	GameplayTags.CombatSocket_Tail = Manager.AddNativeGameplayTag(
		FName("CombatSocket.Tail"),
		FString("Combat Socket for Tail.")
		);
	
	/**
	 * Montage
	*/
	
	GameplayTags.Montage_Attack_1 = Manager.AddNativeGameplayTag(
		FName("Montage.Attack.1"),
		FString("Montage Attack 1.")
		);
	
	GameplayTags.Montage_Attack_2 = Manager.AddNativeGameplayTag(
		FName("Montage.Attack.2"),
		FString("Montage Attack 2.")
		);
	
	GameplayTags.Montage_Attack_3 = Manager.AddNativeGameplayTag(
		FName("Montage.Attack.3"),
		FString("Montage Attack 3.")
		);
	
	GameplayTags.Montage_Attack_4 = Manager.AddNativeGameplayTag(
		FName("Montage.Attack.4"),
		FString("Montage Attack 4.")
		);

	/**
	 * Input Tags
	 */
	
	GameplayTags.InputTag_LMB = Manager.AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button.")
		);
	
	GameplayTags.InputTag_RMB = Manager.AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button.")
		);
	
	GameplayTags.InputTag_Spell_1 = Manager.AddNativeGameplayTag(
		FName("InputTag.Spell_1"),
		FString("Input Tag for Spell 1.")
		);
	
	GameplayTags.InputTag_Spell_2 = Manager.AddNativeGameplayTag(
		FName("InputTag.Spell_2"),
		FString("Input Tag for Spell 2.")
		);
	
	GameplayTags.InputTag_Spell_3 = Manager.AddNativeGameplayTag(
		FName("InputTag.Spell_3"),
		FString("Input Tag for Spell 3.")
		);
	
	GameplayTags.InputTag_Spell_4 = Manager.AddNativeGameplayTag(
		FName("InputTag.Spell_4"),
		FString("Input Tag for Spell 4.")
		);
	
	GameplayTags.InputTag_Passive_1 = Manager.AddNativeGameplayTag(
		FName("InputTag.Passive_1"),
		FString("Input Tag for Passive 1.")
		);
	
	GameplayTags.InputTag_Passive_2 = Manager.AddNativeGameplayTag(
		FName("InputTag.Passive_2"),
		FString("Input Tag for Passive 2.")
		);
	
	/**
	 * Player tags
	*/
	
	GameplayTags.Player_Block_InputPressed = Manager.AddNativeGameplayTag(
		FName("Player.Block.InputPressed"),
		FString("Block Input Pressed callback for input.")
		);
	
	GameplayTags.Player_Block_InputHeld = Manager.AddNativeGameplayTag(
		FName("Player.Block.InputHeld"),
		FString("Block Input Held callback for input.")
		);
	
	GameplayTags.Player_Block_InputReleased = Manager.AddNativeGameplayTag(
		FName("Player.Block.InputReleased"),
		FString("Block Input Released callback for input.")
		);
	
	GameplayTags.Player_Block_CursorTrace = Manager.AddNativeGameplayTag(
		FName("Player.Block.CursorTrace"),
		FString("Block tracing under the cursor.")
		);
	
	GameplayTags.bInitialized = true;
	
}
