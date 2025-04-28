#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPGPlayerController.generated.h"

class UNiagaraSystem;
class UDamageTextComponent;
class USplineComponent;
class URPGAbilitySystemComponent;
struct FGameplayTag;
class URPGInputConfig;
class UInputMappingContext;
class UInputAction;
class IMobInterface;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class RPG_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARPGPlayerController();
	virtual void PlayerTick(float DeltaSeconds) override;

	const FHitResult& GetCursorHit() const { return CursorHit; }

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float Damage, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	void GenerateAutoRunningPath();
	void AutoRun();
	void MoveFollowingMouseLocation();
	void Move(const FInputActionValue& InputActionValue);
	void ShiftPressed();
	void ShiftReleased();
	void CursorTrace();
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> PlayerContext;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> ShiftAction;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<URPGInputConfig> InputConfig;

	UPROPERTY();
	TObjectPtr<URPGAbilitySystemComponent> RPGAbilitySystemComponent;
	URPGAbilitySystemComponent* GetASC();

	TScriptInterface<IMobInterface> LastTargetedActor;
	TScriptInterface<IMobInterface> CurrentTargetedActor;
	FHitResult CursorHit;

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	bool bAutoRunning = false;
	bool bTargetingActor = false;
	bool bClickToMove = false;
	
	UPROPERTY(EditDefaultsOnly, Category = Input);
	float ShortPressThreshold = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = Input);
	float AutoRunAcceptanceRadius = 50.f;
	
	UPROPERTY(VisibleAnywhere);
	TObjectPtr<USplineComponent> Spline = nullptr;
	
	UPROPERTY(EditDefaultsOnly);
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem = nullptr;

	UPROPERTY(EditDefaultsOnly);
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	bool bShiftKeyPressed = false;
};

