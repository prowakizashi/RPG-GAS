#include "AbilitySystem/Abilities/ProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "RPGGameplayTags.h"
#include "Actor/ProjectileBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UProjectileSpell::SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetAttackSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);
	FRotator Rotation = (TargetLocation - SocketLocation).Rotation();
	Rotation.Pitch = 0.0f;
		
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
		
	// We spawn it deferred to get time to init it before
	AProjectileBase* Projectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

	// we don't know the target yet, so we use default parameter (which is nullptr)
	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	
	Projectile->FinishSpawning(SpawnTransform);
}
