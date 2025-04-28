#include "AbilitySystem/AbilityTasks/TargetDataUnderMouseTask.h"

#include "AbilitySystemComponent.h"
#include "Player/RPGPlayerController.h"
#include "RPG/RPG.h"

UTargetDataUnderMouseTask* UTargetDataUnderMouseTask::CreateTargetDataUnderMouseTask(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouseTask* Obj = NewAbilityTask<UTargetDataUnderMouseTask>(OwningAbility);
	return Obj;
}

void UTargetDataUnderMouseTask::Activate()
{
	if (IsLocallyControlled())
	{
		SendMouseCursorData();
	}
	else // We are on the server
	{
		const FGameplayAbilitySpecHandle& SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey& PredictionKey = GetActivationPredictionKey();
		
		// We bind to the reception of the data sent in the function SendMouseCursorData().
		// SpecHandle and PredictionKey act like identifier to bind to the correct reception of data.
		AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey).AddUObject(this,
			&UTargetDataUnderMouseTask::OnTargetDataReplicatedCallback);
		
		// ... But the reception of data may have already occured and the broadcast of the event already done.
		// So we call this function to prevent that case.
		const bool bCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, PredictionKey);

		// ... And if the reception of data didn't occured yet, then the task waits to get it, and the bound function will be called when it occurs
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouseTask::SendMouseCursorData() const
{
	// Sets the "AbilitySystemComponent::ScopedPredictionKey"
	// Says "Everything in the current scope (the scope of this function) should be predicted"
	// This means we want to be allowed what is done here locally, and server will confirm what we've done
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	if (ARPGPlayerController* Controller = Cast<ARPGPlayerController>(Ability->GetCurrentActorInfo()->PlayerController))
	{
		FHitResult CursorHit;
		Controller->GetHitResultUnderCursor(ECC_Target, false, CursorHit);
		
		FGameplayAbilityTargetDataHandle DataHandle;
		FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
		Data->HitResult = CursorHit;
		DataHandle.Add(Data);

		// Send predicted data to the server
		AbilitySystemComponent->ServerSetReplicatedTargetData(
			GetAbilitySpecHandle(),
			GetActivationPredictionKey(),
			DataHandle,
			FGameplayTag(),
			AbilitySystemComponent->ScopedPredictionKey
			);
		
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnGetTargetData.Broadcast(DataHandle);
		}
	}
}

void UTargetDataUnderMouseTask::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag) const
{
	// We remove/clear the cached data the server received, so the server doesn't keep it stored indefinitely
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnGetTargetData.Broadcast(DataHandle);
	}
}
