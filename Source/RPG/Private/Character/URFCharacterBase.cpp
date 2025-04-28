#include "Character/RPGCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "RPGGameplayTags.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/RPG.h"

ARPGCharacterBase::ARPGCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ARPGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARPGCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	Multicast_OnDeath();
}

FVector ARPGCharacterBase::GetAttackSocketLocation_Implementation(const FGameplayTag& MontageTag) const
{
	const auto& GTags = FRPGGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GTags.CombatSocket_Weapon))
	{
		if (IsValid(Weapon))
		{
			return Weapon->GetSocketLocation(WeaponOriginSocketName);
		}
	}
	else if (MontageTag.MatchesTagExact(GTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandOriginSocketName);
	}
	else if (MontageTag.MatchesTagExact(GTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandOriginSocketName);
	}
	else if (MontageTag.MatchesTagExact(GTags.CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailOriginSocketName);
	}

	return GetActorLocation();
}

FTaggedMontage ARPGCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& GameplayTag) const
{
	for (const auto& TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag == GameplayTag)
			return TaggedMontage;
	}
	return FTaggedMontage();
}

USkeletalMeshComponent* ARPGCharacterBase::GetWeapon_Implementation() const
{
	return Weapon;
}

void ARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARPGCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.0f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.0f);
	ApplyEffectToSelf(DefaultVitalsAttributes, 1.0f);
}

void ARPGCharacterBase::Multicast_OnDeath_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	bDead = true;
}

void ARPGCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const
{
	check(GameplayEffectClass);
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(ASC);
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), ASC);
}

void ARPGCharacterBase::AddCharacterAbilities() const
{
	URPGAbilitySystemComponent* RpgASC = CastChecked<URPGAbilitySystemComponent>(GetAbilitySystemComponent());
	if (!HasAuthority()) return;

	RpgASC->AddCharacterAbilities(StartupAbilities);
	RpgASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

void ARPGCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynMatInst);
		StartDissolveTimeline(DynMatInst);
	}
	
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynMatInst);
		StartWeaponDissolveTimeline(DynMatInst);
	}
}

