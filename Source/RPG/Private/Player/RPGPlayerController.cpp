#include "Player/RPGPlayerController.h"

#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SplineComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/RPGInputComponent.h"
#include "Interaction/MobInterface.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "RPGGameplayTags.h"
#include "GameFramework/Character.h"
#include "UI/Widgets/DamageTextComponent.h"

ARPGPlayerController::ARPGPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("SplineComponent");
}

void ARPGPlayerController::PlayerTick(float DeltaSeconds)
{
	Super::PlayerTick(DeltaSeconds);
	CursorTrace();
	AutoRun();
}

void ARPGPlayerController::CursorTrace()
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FRPGGameplayTags::Get().Player_Block_CursorTrace))
	{
		if (LastTargetedActor != nullptr) LastTargetedActor->UnHighlightActor();
		if (CurrentTargetedActor != nullptr) CurrentTargetedActor->HighlightActor();
		LastTargetedActor = nullptr;
		CurrentTargetedActor = nullptr;
		return;
	}
	
	GetHitResultUnderCursor(ECC_Visibility, false , CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastTargetedActor = CurrentTargetedActor;
	CurrentTargetedActor = CursorHit.GetActor();

	if (LastTargetedActor == CurrentTargetedActor) return;
	if (LastTargetedActor != nullptr) LastTargetedActor->UnHighlightActor();
	if (CurrentTargetedActor != nullptr) CurrentTargetedActor->HighlightActor();
}

void ARPGPlayerController::ShowDamageNumber_Implementation(float Damage, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageTextComponent = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageTextComponent->RegisterComponent();
		DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageTextComponent->SetDamageText(Damage, bBlockedHit, bCriticalHit);
	}
}

void ARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(PlayerContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem) // only true on local player controller
	{
		Subsystem->AddMappingContext(PlayerContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	URPGInputComponent* RPGInputComponent = CastChecked<URPGInputComponent>(InputComponent);
	RPGInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::Move);
	RPGInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ARPGPlayerController::ShiftPressed);
	RPGInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ARPGPlayerController::ShiftReleased);
	RPGInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void ARPGPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FRPGGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	
	// TODO: InputTag_LMB is not necessary bound to LMB... This name is confusing and we should refer to the input tag
	// TODO: In future, we should only check if the key triggering the input is a the MLB (or the key used to move the player), not do-able with tags, currently...
	if (InputTag.MatchesTagExact(FRPGGameplayTags::Get().InputTag_LMB))
	{
		bTargetingActor = CurrentTargetedActor != nullptr;
		bAutoRunning = false;
		FollowTime = 0.f;
		bClickToMove = true;
	}

	if (GetASC())
	{
		GetASC()->AbilityInputTagPressed(InputTag);
	}
}

void ARPGPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!GetASC()) return;
	
	if (GetASC()->HasMatchingGameplayTag(FRPGGameplayTags::Get().Player_Block_InputReleased))
	{
		bClickToMove = false;
		return;
	}

	// In any case we notify we released. Handle the case we moved the mouse of the enemy before releasing the input
	GetASC()->AbilityInputTagReleased(InputTag);

	if (bClickToMove && !bShiftKeyPressed && !bTargetingActor && FollowTime < ShortPressThreshold && InputTag.MatchesTagExact(FRPGGameplayTags::Get().InputTag_LMB))
	{
		GenerateAutoRunningPath();
	}
	bClickToMove = false;
}

void ARPGPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FRPGGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	
	// TODO: InputTag_LMB is not necessary bound to LMB... This name is confusing and we should refer to the input tag
	if (bClickToMove && !bShiftKeyPressed && !bTargetingActor && InputTag.MatchesTagExact(FRPGGameplayTags::Get().InputTag_LMB))
	{
		MoveFollowingMouseLocation();
		return;
	}
	
	if (GetASC())
	{
		GetASC()->AbilityInputTagHeld(InputTag);
	}
}

void ARPGPlayerController::GenerateAutoRunningPath()
{
	if (const APawn* ControlledPawn = GetPawn())
	{
		const UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination);
		if (NavPath)
		{
			Spline->ClearSplinePoints();
			for (const FVector& Point : NavPath->PathPoints)
			{
				Spline->AddSplinePoint(Point, ESplineCoordinateSpace::World);
			}

			if (NavPath->PathPoints.Num() > 0)
			{
				// Clicked location could be out of the navmesh, so we set it to be the last point of our PathPoints
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				bAutoRunning = true;
			}
		}
		
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
	}
}

void ARPGPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void ARPGPlayerController::MoveFollowingMouseLocation()
{
	FollowTime += GetWorld()->GetDeltaSeconds();
	if (CursorHit.bBlockingHit)
	{
		CachedDestination = CursorHit.ImpactPoint;
	}

	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection);
	}
}

void ARPGPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FRPGGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (APawn* ControlledPawn = GetPawn<APawn>(); IsValid(ControlledPawn))
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ARPGPlayerController::ShiftPressed()
{
	bShiftKeyPressed = true;
}

void ARPGPlayerController::ShiftReleased()
{
	bShiftKeyPressed = false;
}

URPGAbilitySystemComponent* ARPGPlayerController::GetASC()
{
	if (!RPGAbilitySystemComponent)
	{
		RPGAbilitySystemComponent = Cast<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return RPGAbilitySystemComponent;
}
