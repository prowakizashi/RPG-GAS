#include "AbilitySystem/RPGAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "RPGGameplayTags.h"
#include "RPGLogChannels.h"
#include "AbilitySystem/RPGAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/RPGGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Interaction/PlayerInterface.h"

void URPGAbilitySystemComponent::OnAbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &URPGAbilitySystemComponent::Client_OnEffectApplied);
}

void URPGAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const URPGGameplayAbility* RPGAbility = Cast<URPGGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(RPGAbility->StartupInputTag);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FRPGGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenEvent.Broadcast();
}

void URPGAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility>& PassiveAbilityClass : PassiveAbilities)
	{
		FGameplayAbilitySpec PassiveAbilitySpec = FGameplayAbilitySpec(PassiveAbilityClass, 1);
		GiveAbilityAndActivateOnce(PassiveAbilitySpec);
	}
}

void URPGAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenEvent.Broadcast();
	}
}

void URPGAbilitySystemComponent::Server_SpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecWithAbilityTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>() && IPlayerInterface::Execute_GetSpellPoints(GetAvatarActor()) > 0)
		{
			IPlayerInterface::Execute_RemoveSpellPoints(GetAvatarActor(), 1);
			
			const FRPGGameplayTags& Tags = FRPGGameplayTags::Get();
			FGameplayTag Status = GetStatusFromSpec(*AbilitySpec);
			if (Status.MatchesTagExact(Tags.Abilities_Status_Eligible))
			{
				AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(Tags.Abilities_Status_Eligible);
				AbilitySpec->GetDynamicSpecSourceTags().AddTag(Tags.Abilities_Status_Unlocked);
				Status = Tags.Abilities_Status_Unlocked;
			}
			else if (Status.MatchesTagExact(Tags.Abilities_Status_Equipped) || Status.MatchesTagExact(Tags.Abilities_Status_Unlocked))
			{
				AbilitySpec->Level += 1;
			}
			
			Client_UpdateAbilityStatus(AbilityTag, Status, AbilitySpec->Level);
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		
	}
}

void URPGAbilitySystemComponent::Server_EquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag)
{
	const FRPGGameplayTags& Tags = FRPGGameplayTags::Get();
	
	if (FGameplayAbilitySpec* EquippedAbilitySpec = GetSpecWithAbilityTag(AbilityTag))
	{
		FGameplayTag AbilityTypeTagNeeded;
		GetAbilityTypeTagFromInputTag(InputTag, AbilityTypeTagNeeded);
		
		FGameplayTag AbilityTypeTag;
		if (!GetAbilityTypeTagFromAbilityTag(AbilityTag, AbilityTypeTag)) return;
		if (!AbilityTypeTag.MatchesTagExact(AbilityTypeTagNeeded)) return;

		bool bCanEquip = false;
		const FGameplayTag& Status = GetStatusFromSpec(*EquippedAbilitySpec);
		if (Status.MatchesTagExact(Tags.Abilities_Status_Equipped))
		{
			// If previously / already assigned to an Input. We clear it.
			ClearAbilitySpecInputTag(EquippedAbilitySpec);
			bCanEquip = true;
		}
		if (Status.MatchesTagExact(Tags.Abilities_Status_Unlocked))
		{
			// If not previously / already equipped, we set equipped tag.
			EquippedAbilitySpec->GetDynamicSpecSourceTags().RemoveTag(Tags.Abilities_Status_Unlocked);
			EquippedAbilitySpec->GetDynamicSpecSourceTags().AddTag(Tags.Abilities_Status_Equipped);
			bCanEquip = true;
		}

		if (!bCanEquip) return;

		// If another Ability was already used by this input..
		FGameplayTag UnEquippedAbilityTag;
		if (FGameplayAbilitySpec* UnEquippedAbilitySpec = GetSpecWithDynamicTag(InputTag))
		{
			UnEquippedAbilityTag = GetAbilityTagFromSpec(*UnEquippedAbilitySpec);
			
			ClearAbilitySpecInputTag(UnEquippedAbilitySpec);
			if (UnEquippedAbilitySpec->GetDynamicSpecSourceTags().HasTagExact(Tags.Abilities_Status_Equipped))
			{
				UnEquippedAbilitySpec->GetDynamicSpecSourceTags().RemoveTag(Tags.Abilities_Status_Equipped);
				UnEquippedAbilitySpec->GetDynamicSpecSourceTags().AddTag(Tags.Abilities_Status_Unlocked);
				
				MarkAbilitySpecDirty(*UnEquippedAbilitySpec);
			}
		}

		// We set the input tag
		EquippedAbilitySpec->GetDynamicSpecSourceTags().AddTag(InputTag);
		MarkAbilitySpecDirty(*EquippedAbilitySpec);
		Client_EquipAbility(AbilityTag, UnEquippedAbilityTag, InputTag, EquippedAbilitySpec->Level);
	}
}

void URPGAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	// We lock access to ability list while we are iterating on it
	// To prevent native ability system to make changes during that time
	FScopedAbilityListLock ActiveScopeLock(*this);
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				UGameplayAbility* PrimaryInstance = AbilitySpec.GetPrimaryInstance();
				if (PrimaryInstance)
				{
					// We tell the server that the player has released the input, only works with InstancedPerActor abilities...
					InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, PrimaryInstance->GetCurrentActivationInfo().GetActivationPredictionKey());
				}
			}
			break;
		}
	}
}

void URPGAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	// We lock access to ability list while we are iterating on it
	// To prevent native ability system to make changes during that time
	FScopedAbilityListLock ActiveScopeLock(*this);
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
			break;
		}
	}
}

void URPGAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	// We lock access to ability list while we are iterating on it
	// To prevent native ability system to make changes during that time
	FScopedAbilityListLock ActiveScopeLock(*this);
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				UGameplayAbility* PrimaryInstance = AbilitySpec.GetPrimaryInstance();
				if (PrimaryInstance)
				{
					// We tell the server that the player has released the input, only works with InstancedPerActor abilities...
					InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, PrimaryInstance->GetCurrentActivationInfo().GetActivationPredictionKey());
				}
			}
			break;
		}
	}
}

void URPGAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	// We lock access to ability list while we are iterating on it
	// To prevent native ability system to make changes during that time
	FScopedAbilityListLock ActiveScopeLock(*this);
	
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogRpg, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

void URPGAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag) const
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			Server_UpgradeAttribute(AttributeTag);
		}
	}
}

void URPGAbilitySystemComponent::UpdateAbilityStatuses(int32 NewLevel)
{
	const UAbilityInfo* AbilityInfo = URPGAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FRPGAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (NewLevel < Info.LevelRequirement) continue;
		
		if (GetSpecWithAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FRPGGameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			
			// will force replication to notify the client
			MarkAbilitySpecDirty(AbilitySpec);
			
			Client_UpdateAbilityStatus(Info.AbilityTag, FRPGGameplayTags::Get().Abilities_Status_Eligible, 1);
		}
	}
}

void URPGAbilitySystemComponent::ClearAbilitySpecInputTag(FGameplayAbilitySpec* AbilitySpec)
{
	const FGameplayTag& InputTag = GetInputTagFromSpec(*AbilitySpec);
	AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(InputTag);
	MarkAbilitySpecDirty(*AbilitySpec);
}

void URPGAbilitySystemComponent::ClearAbilitiesWithInputTag(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpec.GetDynamicSpecSourceTags().RemoveTag(InputTag);
		}
	}
}

FGameplayAbilitySpec* URPGAbilitySystemComponent::GetSpecWithAbilityTag(const FGameplayTag& Tag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability->GetAssetTags().HasTagExact(Tag))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

FGameplayAbilitySpec* URPGAbilitySystemComponent::GetSpecWithDynamicTag(const FGameplayTag& Tag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(Tag))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

bool URPGAbilitySystemComponent::GetDescriptionsByAbilityTag(const UAbilityInfo* AbilityInfo, const FGameplayTag& AbilityTag, FString& OutDescription,
	FString& OutNextLevelDescription)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecWithAbilityTag(AbilityTag))
	{
		const URPGGameplayAbility* Ability = Cast<URPGGameplayAbility>(AbilitySpec->Ability);
		OutDescription = Ability->GetDescription(AbilitySpec->Level);
		OutNextLevelDescription = Ability->GetNextLevelDescription(AbilitySpec->Level + 1);
		return true;
	}

	FRPGAbilityInfo Info;
	if (AbilityInfo->FindAbilityInfoForTag(AbilityTag, Info))
	{
		OutDescription = URPGGameplayAbility::GetLockedDescription(Info.LevelRequirement);
		OutNextLevelDescription = FString();
	}
	return false;
}

void URPGAbilitySystemComponent::Server_UpgradeAttribute_Implementation(const FGameplayTag& AttributeTag) const
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			FGameplayEventData Payload;
			Payload.EventTag = AttributeTag;
			Payload.EventMagnitude = 1.f;

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
			IPlayerInterface::Execute_RemoveAttributePoints(GetAvatarActor(), 1);
		}
	}
}

FGameplayTag URPGAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (const FGameplayTag& StatusTag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayTag URPGAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (const FGameplayTag& Tag : AbilitySpec.Ability->GetAssetTags())
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
				return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag URPGAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (const FGameplayTag& Tag : AbilitySpec.GetDynamicSpecSourceTags())
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
				return Tag;
		}
	}
	return FGameplayTag();
}

bool URPGAbilitySystemComponent::GetAbilityTypeTagFromInputTag(const FGameplayTag& InputTag, FGameplayTag& OutAbilityTypeTag)
{
	if (!InputTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag")))) return false;

	const FRPGGameplayTags& Tags = FRPGGameplayTags::Get();
	if (InputTag.MatchesTag(Tags.InputTag_Passive_1) || InputTag.MatchesTag(Tags.InputTag_Passive_2))
	{
		OutAbilityTypeTag = Tags.Abilities_Type_Passive;
		return true;
	}
	
	OutAbilityTypeTag = Tags.Abilities_Type_Offensive;
	return true;
}

bool URPGAbilitySystemComponent::GetAbilityTypeTagFromAbilityTag(const FGameplayTag& AbilityTag, FGameplayTag& OutAbilityTypeTag) const
{
	const UAbilityInfo* AbilityInfo = URPGAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());

	FRPGAbilityInfo Info;
	if (!AbilityInfo->FindAbilityInfoForTag(AbilityTag, Info)) return false;

	OutAbilityTypeTag = Info.AbilityType;
	return true;
}

void URPGAbilitySystemComponent::Client_OnEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                       const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const
{
	//GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, FString("Effect Applied!"));
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTagsEvent.Broadcast(TagContainer);
}

void URPGAbilitySystemComponent::Client_UpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag, int32 AbilityLevel) const
{
	AbilitiesStatusChangedEvent.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

void URPGAbilitySystemComponent::Client_EquipAbility_Implementation(const FGameplayTag& EquippedAbilityTag,
	const FGameplayTag& UnEquippedAbilityTag, const FGameplayTag& InputTag, int32 AbilityLevel) const
{
	AbilitiesEquippedEvent.Broadcast(EquippedAbilityTag, UnEquippedAbilityTag, InputTag, AbilityLevel);
}