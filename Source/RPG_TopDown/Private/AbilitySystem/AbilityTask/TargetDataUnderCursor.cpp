// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/TargetDataUnderCursor.h"

#include "AbilitySystemComponent.h"


UTargetDataUnderCursor* UTargetDataUnderCursor::CreateTargetDataUnderCursor(UGameplayAbility* OwningGameplayAbility)
{
	UTargetDataUnderCursor* MyObj = NewAbilityTask<UTargetDataUnderCursor>(OwningGameplayAbility);
	return MyObj;
}

void UTargetDataUnderCursor::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendTargetDataToServer();
	}
	else
	{
		ReceiveTargetDataFromClient();
	}
}

// Implements the SendCursorData method, which gathers data from the cursor and sends it to the server if necessary.
void UTargetDataUnderCursor::SendTargetDataToServer()
{
	// Opens a scoped prediction window to handle prediction logic for networked gameplay.
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	const APlayerController* Controller = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHitResult;
	if (Controller)
	{
		Controller->GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);
	}

	FGameplayAbilityTargetDataHandle TargetDataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit();
	TargetData->HitResult = CursorHitResult;
	TargetDataHandle.Add(TargetData);

	// Sends the target data to the server for replication.
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(),
		TargetDataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);

	// If the ability task delegates should be broadcasted, it broadcasts the ValidTargetDataHandle delegate with the target data handle.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidTargetDataHandle.Broadcast(TargetDataHandle);
	}
}

// Implements the ReceiveTargetDataFromClient method, which listens for target data sent from the client.
void UTargetDataUnderCursor::ReceiveTargetDataFromClient()
{
	// Registers a callback for when the target data is replicated.
	AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(),
															   GetActivationPredictionKey()).AddUObject(this, &UTargetDataUnderCursor::OnTargetDataReplicatedCallback);
	
	const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (!bCalledDelegate)
	{
		SetWaitingOnRemotePlayerData();
	}
}

void UTargetDataUnderCursor::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	
	// If the ability task delegates should be broadcasted, it broadcasts the ValidTargetDataHandle delegate with the target data handle.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidTargetDataHandle.Broadcast(DataHandle);
	}
}
