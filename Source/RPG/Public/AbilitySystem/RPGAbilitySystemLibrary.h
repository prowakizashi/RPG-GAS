#pragma once

#include "CoreMinimal.h"
#include "RPGAbilityTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGAbilitySystemLibrary.generated.h"

class UAbilityInfo;
struct FWidgetControllerParams;
class UAttributeMenuWidgetController;
class USpellMenuWidgetController;
struct FGameplayEffectSpecHandle;
struct FGameplayEffectContextHandle;
class UCharacterClassInfo;
enum class ECharacterClass : uint8;
class UOverlayWidgetController;


struct FActorDistance
{
	AActor* Actor;
	float DistanceSquared;

	FActorDistance(AActor* InActor, float InDistanceSquared)
		: Actor(InActor), DistanceSquared(InDistanceSquared)
	{}

	// Max-heap: farther actor = higher priority (because we remove the farther actor from the list of closest ones)
	bool operator<(const FActorDistance& Other) const
	{
		return DistanceSquared > Other.DistanceSquared;
	}
};

/**
 * 
 */
UCLASS()
class RPG_API URPGAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="RPG Ability System|Widget Controller", meta=(WorldContext = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, ARPGHUD*& OutHUD);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="RPG Ability System|Widget Controller", meta=(WorldContext = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="RPG Ability System|Widget Controller", meta=(WorldContext = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="RPG Ability System|Widget Controller", meta=(WorldContext = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category="RPG Ability System|Character Class Defaults", meta=(WorldContext = "WorldContextObject"))
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent);

	// Give Startup Abilities such as "Hit React Ability", etc
	UFUNCTION(BlueprintCallable, Category="RPG Ability System|Character Class Defaults", meta=(WorldContext = "WorldContextObject"))
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category="RPG Ability System|Character Class Defaults", meta=(WorldContext = "WorldContextObject"))
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="RPG Ability System|Character Class Defaults", meta=(WorldContext = "WorldContextObject"))
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="RPG Ability System|Gameplay Effects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="RPG Ability System|Gameplay Effects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="RPG Ability System|Gameplay Effects")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="RPG Ability System|Gameplay Effects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="RPG Ability System|Gameplay Effects")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="RPG Ability System|Gameplay Effects")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="RPG Ability System|Gameplay Effects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="RPG Ability System|Gameplay Effects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);
	
	UFUNCTION(BlueprintCallable, Category="RPG Ability System|Gameplay Effects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);
	
	UFUNCTION(BlueprintCallable, Category="RPG Ability System|Gameplay Effects")
	static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="RPG Ability System|Gameplay Effects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="RPG Ability System|Gameplay Effects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="RPG Ability System|Gameplay Effects")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="RPG Ability System|Gameplay Effects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, FGameplayTag const& InDamageType);

	UFUNCTION(BlueprintCallable, Category="RPG Ability System|Gameplay Mechanics", meta=(WorldContext = "WorldContextObject"))
	static void GetAlivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingPlayers, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);
	
	UFUNCTION(BlueprintCallable, Category="RPG Ability System|Gameplay Mechanics", meta=(WorldContext = "WorldContextObject"))
	static void GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="RPG Ability System|Gameplay Mechanics")
	static bool AreNotFriends(const AActor* FirstActor, const AActor* SecondActor);
	
	UFUNCTION(BlueprintCallable, Category="RPG Ability System|Damage Effect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams DamageEffectParams);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="RPG Ability System|Gameplay Mechanics", meta=(WorldContext = "WorldContextObject"))
	static int32 GetXpRewardForClassAndReward(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);

};
