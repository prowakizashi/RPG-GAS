#include "AbilitySystem/Abilities/SummonSpell.h"

TArray<FVector> USummonSpell::GetSpawnLocations()
{
	TArray<FVector> SpawnLocations;
	const FVector& Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector& Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / (NumSummoned + 1);
	
	const FVector& RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);
	const FVector TraceOffset = FVector::UpVector * 400; 
	for (int32 i = 1; i <= NumSummoned; ++i)
	{
		const FVector& Direction = RightOfSpread.RotateAngleAxis(-DeltaSpread * i, FVector::UpVector);
		FVector ChoseSpawnLocation = Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChoseSpawnLocation + TraceOffset, ChoseSpawnLocation - TraceOffset, ECC_Visibility);
		if (Hit.bBlockingHit)
			ChoseSpawnLocation = Hit.ImpactPoint;
		SpawnLocations.Add(ChoseSpawnLocation);
	}
	
	return SpawnLocations;
}

TSubclassOf<APawn> USummonSpell::GetRandomSummonedClass() const
{
	return SummonedClasses[FMath::RandRange(0, SummonedClasses.Num() - 1)];
}
