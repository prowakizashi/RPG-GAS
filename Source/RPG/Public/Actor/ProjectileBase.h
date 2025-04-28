#pragma once

#include "CoreMinimal.h"
#include "RPGAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class RPG_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
private:
	
	void Explodes(const FVector& InHitLocation);
		
	UFUNCTION(NetMulticast, Unreliable)
	void Client_Explodes(const FVector& InHitLocation);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> ImpactSound;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> TravelSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> TravelSoundComponent;

	bool bHit = false;
	FTimerHandle TimerHandle;
};
