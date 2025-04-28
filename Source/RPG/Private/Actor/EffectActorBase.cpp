#include "Actor/EffectActorBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


AEffectActorBase::AEffectActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot")));
}

void AEffectActorBase::BeginPlay()
{
	Super::BeginPlay();

}

void AEffectActorBase::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	if (TargetActor->ActorHasTag(FName("Mob")) && !bApplyEffectsToEnnemies) return;
	
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		check(GameplayEffectClass);
		FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
		const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);

		const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
		if (bIsInfinite)
		{
			bool bRemoveOnEndOverlap = true;
			for (const FInfiniteGameplayEffectApplicationInfo& EffectInfo : InfiniteGameplayEffects)
			{
				if (EffectInfo.GameplayEffectClass == GameplayEffectClass)
				{
					bRemoveOnEndOverlap = EffectInfo.EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap;
					break;
				}
			}
			
			if (bRemoveOnEndOverlap)
			{
				ActiveEffectHandles.Add(ActiveEffectHandle, FInfiniteGameplayToRemoveInfo(GameplayEffectClass, TargetASC));
			}
		}
		else if (bDestroyOnEffectApplication)
		{
			// Should we really do this since this actor can applies multiple combined effects ?
			Destroy();
		}
	}

}

void AEffectActorBase::OnStartOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Mob")) && !bApplyEffectsToEnnemies) return;
	
	for (const FGameplayEffectApplicationInfo& EffectInfo : InstantGameplayEffects)
	{
		if (EffectInfo.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, EffectInfo.GameplayEffectClass);
		}
	}
	
	for (const FGameplayEffectApplicationInfo& EffectInfo : DurationGameplayEffects)
	{
		if (EffectInfo.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, EffectInfo.GameplayEffectClass);
		}
	}
	
	for (const FInfiniteGameplayEffectApplicationInfo& EffectInfo : InfiniteGameplayEffects)
	{
		if (EffectInfo.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, EffectInfo.GameplayEffectClass);
		}
	}
}

void AEffectActorBase::OnEndOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Mob")) && !bApplyEffectsToEnnemies) return;
	
	for (const FGameplayEffectApplicationInfo& EffectInfo : InstantGameplayEffects)
	{
		if (EffectInfo.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, EffectInfo.GameplayEffectClass);
		}
	}
	
	for (const FGameplayEffectApplicationInfo& EffectInfo : DurationGameplayEffects)
	{
		if (EffectInfo.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, EffectInfo.GameplayEffectClass);
		}
	}
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	for (const FInfiniteGameplayEffectApplicationInfo& EffectInfo : InfiniteGameplayEffects)
	{
		if (EffectInfo.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, EffectInfo.GameplayEffectClass);
		}
	
		if (EffectInfo.EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			if (!IsValid(TargetASC)) continue;

			TArray<FActiveGameplayEffectHandle> HandlesToRemove;
			for (const TTuple<FActiveGameplayEffectHandle, FInfiniteGameplayToRemoveInfo>& HandlePair : ActiveEffectHandles)
			{
				if (TargetASC == HandlePair.Value.ASC && EffectInfo.GameplayEffectClass == HandlePair.Value.GameplayEffectClass)
				{
					TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
					HandlesToRemove.Add(HandlePair.Key);
				}
			}

			for (const FActiveGameplayEffectHandle& Handle : HandlesToRemove)
			{
				ActiveEffectHandles.FindAndRemoveChecked(Handle);
			}
		}
	}
}
