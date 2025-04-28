#include "Actor/ProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "RPGGameplayTags.h"
#include "AbilitySystem/RPGAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"
#include "RPG/RPG.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550;
	ProjectileMovement->MaxSpeed = 550;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);
	
	if (HasAuthority())
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnSphereOverlap);
	}
	else
	{
		TravelSoundComponent = UGameplayStatics::SpawnSoundAttached(TravelSound, GetRootComponent());
		if (TravelSoundComponent)
		{
			TravelSoundComponent->bStopWhenOwnerDestroyed = true;
		}
	}
}

void AProjectileBase::BeginDestroy()
{
	Super::BeginDestroy();

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void AProjectileBase::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Should never be false
	if (!HasAuthority()) return;

	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == nullptr) return;

	// Can't touch ourself
	if (SourceAvatarActor == OtherActor) return;
	
	// No Friendly Fire
	if (!URPGAbilitySystemLibrary::AreNotFriends(SourceAvatarActor, OtherActor)) return;

	// Notify the client 
	Client_Explodes(GetActorLocation());
	
	// Disable the projectile
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovement->MaxSpeed = 0;
	
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		// It wasn't set yet, because we didn't the target before hitting an enemy
		DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
		URPGAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
	}

	// We delay the destruction of the projectile to let client have time to receive info about the hit (replication process)
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]
	{
		if (!IsValid(this)) return;
		Destroy();
	}, 1.f, false);
}

void AProjectileBase::Client_Explodes_Implementation(const FVector& InHitLocation)
{
	Explodes(InHitLocation);
}

void AProjectileBase::Explodes(const FVector& InHitLocation)
{
	if (bHit) return;

	bHit = true;
	SetActorHiddenInGame(true);
	if (TravelSoundComponent)
		TravelSoundComponent->Stop();
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
}
