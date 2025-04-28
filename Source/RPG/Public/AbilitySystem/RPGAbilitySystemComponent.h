#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RPGAbilitySystemComponent.generated.h"

class UAbilityInfo;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* AssetTags */);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec& /* AbilitySpec */);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*Ability Tag*/, const FGameplayTag& /*Status Tag*/, int32 /*AbilityLevel*/)
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquippedSignature, const FGameplayTag& /*Equipped Ability Tag*/, const FGameplayTag& /*Unequipped Status Tag*/, const FGameplayTag& /*Input Tag*/, int32 /*AbilityLevel*/)

/**
 *  Use "showdebug abilitysystem" command to get info during PIE
 */
UCLASS()
class RPG_API URPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:

	void OnAbilityActorInfoSet();
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities);
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbility& Delegate);
	void UpgradeAttribute(const FGameplayTag& AttributeTag) const;
	void UpdateAbilityStatuses(int32 NewLevel);
	
	void ClearAbilitySpecInputTag(FGameplayAbilitySpec* AbilitySpec);
	void ClearAbilitiesWithInputTag(const FGameplayTag& InputTag);

	FGameplayAbilitySpec* GetSpecWithAbilityTag(const FGameplayTag& Tag);
	FGameplayAbilitySpec* GetSpecWithDynamicTag(const FGameplayTag& Tag);

	bool GetDescriptionsByAbilityTag(const UAbilityInfo* AbilityInfo, const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);

	UFUNCTION(Server, Reliable)
	void Server_UpgradeAttribute(const FGameplayTag& AttributeTag) const;
	UFUNCTION(Server, Reliable)
	void Server_SpendSpellPoint(const FGameplayTag& AbilityTag);
	UFUNCTION(Server, Reliable)
	void Server_EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag);
	
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static bool GetAbilityTypeTagFromInputTag(const FGameplayTag& InputTag, FGameplayTag& OutAbilityTypeTag);
	bool GetAbilityTypeTagFromAbilityTag(const FGameplayTag& AbilityTag, FGameplayTag& OutAbilityTypeTag) const;

	FEffectAssetTags EffectAssetTagsEvent;
	FAbilitiesGiven AbilitiesGivenEvent;
	FAbilityStatusChanged AbilitiesStatusChangedEvent;
	FAbilityEquippedSignature AbilitiesEquippedEvent;

	bool bStartupAbilitiesGiven = false;

	
protected:
	
	virtual void OnRep_ActivateAbilities() override;

	UFUNCTION(Client, Reliable)
	void Client_OnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const;

	UFUNCTION(Client, Reliable)
	void Client_UpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel) const;
	
	UFUNCTION(Client, Reliable)
	void Client_EquipAbility(const FGameplayTag& EquippedAbilityTag, const FGameplayTag& UnEquippedAbilityTag, const FGameplayTag& InputTag, int32 AbilityLevel) const;
};
