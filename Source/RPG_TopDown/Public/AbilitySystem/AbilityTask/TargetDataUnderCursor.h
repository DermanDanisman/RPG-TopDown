// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderCursor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetDataUnderCursorSignature, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API UTargetDataUnderCursor : public UAbilityTask
{
	GENERATED_BODY()

public:

	/*
	* This is now an asynchronous blueprint node, which means it can have multiple output execution pins.
	* And the way that we create those multiple output execution pins is by broadcasting delegates.
	* So any output execution pin needs to exist as a delegate variable on this class.
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(DisplayName="GetTargetDataUnderCursor", HidePin = "OwningGameplayAbility", DefaultToSelf = "OwningGameplayAbility", BlueprintInternalUseOnly="true"))
    static UTargetDataUnderCursor* CreateTargetDataUnderCursor(UGameplayAbility* OwningGameplayAbility);

	/**
	 * Purpose:
	 * Declares a BlueprintAssignable property of type FTargetDataUnderCursorSignature named ValidTargetDataHandle.
	 * This delegate can be bound to functions in Blueprints and will be broadcasted when valid target data is obtained.
	 */
	UPROPERTY(BlueprintAssignable)
	FTargetDataUnderCursorSignature ValidTargetDataHandle;

private:

	// In order to make use of ValidHitResult execution pin, we need to broadcast it. So, we can get valid hit result.
	// And in order to do that, we have to override the ability task's Activate() function, and as soon as this node is executed, Activate() will be called.
	virtual void Activate() override;

	// Declares a private method to send cursor data to the server.
	void SendTargetDataToServer();
	// Declares a private method to receive target data from the client.
	void ReceiveTargetDataFromClient();

	// Declares a private method that handles the callback when target data is replicated.
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
