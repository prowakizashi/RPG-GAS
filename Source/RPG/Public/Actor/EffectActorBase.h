#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "EffectActorBase.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};

USTRUCT(BlueprintType)
struct RPG_API FGameplayEffectApplicationInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy EffectApplicationPolicy = EEffectApplicationPolicy::ApplyOnOverlap;
};

USTRUCT(BlueprintType)
struct FInfiniteGameplayEffectApplicationInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy EffectApplicationPolicy = EEffectApplicationPolicy::ApplyOnOverlap;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectRemovalPolicy EffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;
};

USTRUCT()
struct RPG_API FInfiniteGameplayToRemoveInfo
{
	GENERATED_BODY()
public:
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	TObjectPtr<UAbilitySystemComponent> ASC;
};

UCLASS()
class RPG_API AEffectActorBase : public AActor
{
	GENERATED_BODY()

public:
	AEffectActorBase();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnStartOverlap(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectApplication = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bApplyEffectsToEnnemies = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TArray<FGameplayEffectApplicationInfo> InstantGameplayEffects;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TArray<FGameplayEffectApplicationInfo> DurationGameplayEffects;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TArray<FInfiniteGameplayEffectApplicationInfo> InfiniteGameplayEffects;

	TMap<FActiveGameplayEffectHandle, FInfiniteGameplayToRemoveInfo> ActiveEffectHandles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	float ActorLevel = 1.f;
};
