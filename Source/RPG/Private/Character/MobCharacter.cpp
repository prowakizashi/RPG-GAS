#include "Character/MobCharacter.h"

#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAbilitySystemLibrary.h"
#include "GameplayTags.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/Widgets/RPGUserWidget.h"
#include "RPGGameplayTags.h"
#include "AI/MobAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RPG/RPG.h"

AMobCharacter::AMobCharacter()
{
	// Enables the outline when placing the cursor on the enemy
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);

	AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
	AttributeSet = CreateDefaultSubobject<URPGAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AMobCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// AI only managed server side
	if (!HasAuthority()) return;
	
	MobAIController = Cast<AMobAIController>(NewController);
	MobAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	MobAIController->RunBehaviorTree(BehaviorTree);
	MobAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	MobAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

void AMobCharacter::HighlightActor()
{
	bHighlighted = true;
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	if (IsValid(Weapon))
	{
		Weapon->SetRenderCustomDepth(true);
		Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
	
}

void AMobCharacter::UnHighlightActor()
{
	bHighlighted = false;
	GetMesh()->SetRenderCustomDepth(false);
	if (IsValid(Weapon))
	{
		Weapon->SetRenderCustomDepth(false);
	}
}

void AMobCharacter::Die()
{
	Super::Die();
	if (MobAIController)
	{
		MobAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), true);
	}
	SetLifeSpan(LifeSpan);
}

int32 AMobCharacter::GetCharacterLevel_Implementation() const
{
	return Level;
}

void AMobCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();

	if (HasAuthority())
	{
		URPGAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}

	if (URPGUserWidget* UserWidget = Cast<URPGUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		UserWidget->SetWidgetController(this);
	}
	
	if (URPGAttributeSet* RpgAS = CastChecked<URPGAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RpgAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RpgAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});
		
		AbilitySystemComponent->RegisterGameplayTagEvent(FRPGGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved)
			.AddUObject(this, &AMobCharacter::HitReactTagChanged);
		
		OnHealthChanged.Broadcast(RpgAS->GetHealth());
		OnMaxHealthChanged.Broadcast(RpgAS->GetMaxHealth());
	}
}

void AMobCharacter::InitializeDefaultAttributes() const
{
	URPGAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AMobCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<URPGAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoSet();

	if (HasAuthority())
	{
		// TODO: We will have specific attributes for mobs
		InitializeDefaultAttributes();
	}
}

void AMobCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0 : BaseWalkSpeed;
	if (MobAIController && MobAIController->GetBlackboardComponent())
	{
		MobAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}
