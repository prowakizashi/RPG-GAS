#include "Character/RPGPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/RPGPlayerController.h"
#include "Player/RPGPlayerState.h"
#include "UI/HUD/RPGHUD.h"

ARPGPlayerCharacter::ARPGPlayerCharacter()
{
	CharacterClass = ECharacterClass::Elementalist;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->bDoCollisionTest = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void ARPGPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

/** At this time we know that Player State is valid on local player, so should also be the Player Controller */
void ARPGPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

int32 ARPGPlayerCharacter::GetCharacterLevel_Implementation() const
{
	const ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	return RPGPlayerState->GetCharacterLevel();
}

int32 ARPGPlayerCharacter::GetXp_Implementation() const
{
	const ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	return RPGPlayerState->GetCharacterXp();
}

int32 ARPGPlayerCharacter::GetAttributePoints_Implementation() const
{
	const ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	return RPGPlayerState->GetAttributePoints();
}

int32 ARPGPlayerCharacter::GetSpellPoints_Implementation() const
{
	const ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	return RPGPlayerState->GetSpellPoints();
}

int32 ARPGPlayerCharacter::FindLevelForXP_Implementation(int32 XP) const 
{
	const ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	return RPGPlayerState->LevelUpInfo->FindLevelIndexForXP(XP) + 1;
}

FRpgLevelUpInfo ARPGPlayerCharacter::GetLevelUpInfo_Implementation(int32 Level) const
{
	const ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	return RPGPlayerState->LevelUpInfo->LevelUpInformation[Level - 1];
}

void ARPGPlayerCharacter::RemoveAttributePoints_Implementation(int32 Points)
{
	ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	return RPGPlayerState->AddAttributePoints(-Points);
}

void ARPGPlayerCharacter::RemoveSpellPoints_Implementation(int32 Points)
{
	ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	return RPGPlayerState->AddSpellPoints(-Points);
}

void ARPGPlayerCharacter::AddToXp_Implementation(int32 XP)
{
	ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	RPGPlayerState->AddXp(XP);
}

void ARPGPlayerCharacter::LevelUp_Implementation(int32 InPlayerLevelUps, int32 InAttributePoints, int32 InSpellPoints)
{
	ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	RPGPlayerState->AddToLevel(InPlayerLevelUps);
	RPGPlayerState->AddAttributePoints(InAttributePoints);
	RPGPlayerState->AddSpellPoints(InSpellPoints);

	Multicast_LevelUpParticles();

	URPGAbilitySystemComponent* ASC = Cast<URPGAbilitySystemComponent>(GetAbilitySystemComponent());
	ASC->UpdateAbilityStatuses(RPGPlayerState->GetCharacterLevel());
}

void ARPGPlayerCharacter::Multicast_LevelUpParticles_Implementation()
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = Camera->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator Rotation = (CameraLocation - NiagaraSystemLocation).Rotation();

		LevelUpNiagaraComponent->SetWorldRotation(Rotation);
		LevelUpNiagaraComponent->Activate(true);
	}
	
}

void ARPGPlayerCharacter::InitAbilityActorInfo()
{
	ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	AbilitySystemComponent = RPGPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(RPGPlayerState, this);
	Cast<URPGAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoSet();
	AttributeSet = RPGPlayerState->GetAttributeSet();

	ARPGPlayerController* PlayerController = Cast<ARPGPlayerController>(GetController());
	if (IsValid(PlayerController)) // then it's a local player or server
	{
		//TODO: Shouldn't be done on client side only ? GetHUD() probably returns nullptr, server side
		if (ARPGHUD* RPGHUD = Cast<ARPGHUD>(PlayerController->GetHUD()))
		{
			RPGHUD->InitOverlay(PlayerController, RPGPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	//TODO: Shouldn't be done on server side only ?
	//TODO: Who has Authority here, btw ?
	InitializeDefaultAttributes();
}
