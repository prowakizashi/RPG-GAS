#include "AbilitySystem/RPGAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "RPGAbilityTypes.h"
#include "RPGGameplayTags.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Engine/OverlapResult.h"
#include "Game/RPGGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RPGPlayerState.h"
#include "UI/HUD/RPGHUD.h"
#include "UI/WidgetControllers/WidgetControllerBase.h"

bool URPGAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, ARPGHUD*& OutHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0); IsValid(PC))
	{
		if (OutHUD = Cast<ARPGHUD>(PC->GetHUD()); IsValid(OutHUD))
		{
			if (ARPGPlayerState* PS = PC->GetPlayerState<ARPGPlayerState>(); IsValid(PS))
			{
				OutWCParams.AttributeSet = PS->GetAttributeSet();
				OutWCParams.AbilitySystemComponent = PS->GetAbilitySystemComponent();
				OutWCParams.PlayerController = PC;
				OutWCParams.PlayerState = PS;
				return true;
			}
		}
	}
	return false;
}

UOverlayWidgetController* URPGAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams Params;
	ARPGHUD* RpgHUD = nullptr;
	return MakeWidgetControllerParams(WorldContextObject, Params, RpgHUD) ? RpgHUD->GetOverlayWidgetController(Params) : nullptr;
}

UAttributeMenuWidgetController* URPGAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams Params;
	ARPGHUD* RpgHUD = nullptr;
	return MakeWidgetControllerParams(WorldContextObject, Params, RpgHUD) ? RpgHUD->GetAttributeMenuWidgetController(Params) : nullptr;
}

USpellMenuWidgetController* URPGAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams Params;
	ARPGHUD* RpgHUD = nullptr;
	return MakeWidgetControllerParams(WorldContextObject, Params, RpgHUD) ? RpgHUD->GetSpellMenuWidgetController(Params) : nullptr;
}

void URPGAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent)
{
	if (const UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		const FCharacterClassDefaultInfo& DefaultInfo = ClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
		const AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();

		FGameplayEffectContextHandle PrimaryContextHandle = AbilitySystemComponent->MakeEffectContext();
		PrimaryContextHandle.AddSourceObject(AvatarActor);
		const FGameplayEffectSpecHandle& PrimarySpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultInfo.PrimaryAttributeEffect, Level, PrimaryContextHandle);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PrimarySpecHandle.Data);
	
		FGameplayEffectContextHandle SecondaryContextHandle = AbilitySystemComponent->MakeEffectContext();
		SecondaryContextHandle.AddSourceObject(AvatarActor);
		const FGameplayEffectSpecHandle& SecondarySpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ClassInfo->SecondaryAttributeEffect, Level, SecondaryContextHandle);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondarySpecHandle.Data);
	
		FGameplayEffectContextHandle VitalsContextHandle = AbilitySystemComponent->MakeEffectContext();
		VitalsContextHandle.AddSourceObject(AvatarActor);
		const FGameplayEffectSpecHandle& VitalsSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ClassInfo->VitalAttributeEffect, Level, VitalsContextHandle);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalsSpecHandle.Data);
	}
}

void URPGAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject,
	UAbilitySystemComponent* AbilitySystemComponent, ECharacterClass CharacterClass)
{
	if (const UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : ClassInfo->CommonAbilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
		
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : ClassInfo->GetCharacterClassDefaultInfo(CharacterClass).StartupAbilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetCharacterLevel(AbilitySystemComponent->GetAvatarActor()));
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}
}

int32 URPGAbilitySystemLibrary::GetXpRewardForClassAndReward(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, int32 CharacterLevel)
{
	if (const UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		const FCharacterClassDefaultInfo& ClassDefaultInfo = ClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
		return static_cast<int32>(ClassDefaultInfo.XpReward.GetValueAtLevel(CharacterLevel));
	}
	return 0;
}

UCharacterClassInfo* URPGAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	if (const ARPGGameModeBase* GM = Cast<ARPGGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return GM->CharacterClassInfo;
	}
	return nullptr;
}

UAbilityInfo* URPGAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	if (const ARPGGameModeBase* GM = Cast<ARPGGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return GM->AbilityInfo;
	}
	return nullptr;
}

bool URPGAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRPGGameplayEffectContext* EffectContext = static_cast<const FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->IsBlockedHit();
	}
	return false;
}

bool URPGAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRPGGameplayEffectContext* EffectContext = static_cast<const FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->IsCriticalHit();
	}
	return false;
}

bool URPGAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRPGGameplayEffectContext* EffectContext = static_cast<const FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float URPGAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRPGGameplayEffectContext* EffectContext = static_cast<const FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float URPGAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRPGGameplayEffectContext* EffectContext = static_cast<const FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float URPGAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRPGGameplayEffectContext* EffectContext = static_cast<const FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag URPGAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRPGGameplayEffectContext* EffectContext = static_cast<const FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (EffectContext->GetDamageType().IsValid())
			return *EffectContext->GetDamageType();
	}
	return FGameplayTag();
}

void URPGAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FRPGGameplayEffectContext* EffectContext = static_cast<FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void URPGAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FRPGGameplayEffectContext* EffectContext = static_cast<FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void URPGAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff)
{
	if (FRPGGameplayEffectContext* EffectContext = static_cast<FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->SetIsSuccessfulDebuff(bInIsSuccessfulDebuff);
	}
}

void URPGAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage)
{
	if (FRPGGameplayEffectContext* EffectContext = static_cast<FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->SetDebuffDamage(InDebuffDamage);
	}
}

void URPGAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration)
{
	if (FRPGGameplayEffectContext* EffectContext = static_cast<FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->SetDebuffDuration(InDebuffDuration);
	}
}

void URPGAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency)
{
	if (FRPGGameplayEffectContext* EffectContext = static_cast<FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->SetDebuffFrequency(InDebuffFrequency);
	}
}

void URPGAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, FGameplayTag const& InDamageType)
{
	if (FRPGGameplayEffectContext* EffectContext = static_cast<FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->SetDamageType(MakeShared<FGameplayTag>(InDamageType));
	}
}

void URPGAbilitySystemLibrary::GetAlivePlayersWithinRadius(const UObject* WorldContextObject,
                                                           TArray<AActor*>& OutOverlappingPlayers, const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                           const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (const FOverlapResult& Overlap : Overlaps)
		{
			AActor* Actor = Overlap.GetActor();
			if (!Actor->Implements<UCombatInterface>() || ICombatInterface::Execute_IsDead(Actor))
				continue;
			OutOverlappingPlayers.AddUnique(ICombatInterface::Execute_GetAvatar(Actor));
		}
	}
}

void URPGAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors,
	TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{

	if (Actors.Num() <= MaxTargets)
	{
		OutClosestTargets = Actors;
		return;
	}

	// A list of struct that will keep at index 0, the father actor from the closest stored actors.
	TArray<FActorDistance> Heap;
	Heap.Reserve(MaxTargets);
	for (AActor* Actor : Actors)
	{
		if (!Actor) continue;

		float DistSquared = FVector::DistSquared(Origin, Actor->GetActorLocation());
		FActorDistance Entry(Actor, DistSquared);

		if (Heap.Num() < MaxTargets)
		{
			Heap.HeapPush(Entry); 
		}
		else if (DistSquared < Heap[0].DistanceSquared)
		{
			Heap.HeapPopDiscard();
			Heap.HeapPush(Entry);
		}
	}

	for (const FActorDistance& Entry : Heap)
	{
		OutClosestTargets.Add(Entry.Actor);
	}
}

bool URPGAbilitySystemLibrary::AreNotFriends(const AActor* FirstActor, const AActor* SecondActor)
{
	if (!IsValid(FirstActor) || !IsValid(SecondActor))
		return false;
	const bool bBothArePlayers = FirstActor->ActorHasTag("Player") && SecondActor->ActorHasTag("Player");
	const bool bBothAreMobs = FirstActor->ActorHasTag("Mob") && SecondActor->ActorHasTag("Mob");
	return !bBothArePlayers && !bBothAreMobs;
}

FGameplayEffectContextHandle URPGAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams DamageEffectParams)
{
	const FRPGGameplayTags& GameplayTags = FRPGGameplayTags::Get();
	const AActor* AvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle EffectSpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);
	
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
	return EffectContextHandle;
}
