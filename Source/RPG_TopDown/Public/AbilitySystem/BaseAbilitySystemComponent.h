// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemComponent.generated.h"

// Created Delegate for Widget Controller communication.
DECLARE_MULTICAST_DELEGATE_OneParam(FGameplayEffectAssetTags, const FGameplayTagContainer& /* Asset Tags */);

/**
 * UBaseAbilitySystemComponent
 * This class extends UAbilitySystemComponent to add custom functionality for managing abilities and gameplay effects.
 */
UCLASS()
class RPG_TOPDOWN_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	// Function to bind the gameplay effect delegates, This is also called Ability Actor Info Set.
	void BindOnGameplayEffectAppliedDelegateToSelf();

	// This function takes an array of ability classes and adds them to the Ability System Component.
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	// Functions to activate abilities based on input tags
	void ActivateAbilityInputTagHeld(const FGameplayTag& InputTag);
	void ActivateAbilityInputTagReleased(const FGameplayTag& InputTag);

	FGameplayEffectAssetTags GameplayEffectAssetTags;
	
protected:

	/*
	 * Purpose: FOnGameplayEffectAppliedDelegate OnGameplayEffectAppliedDelegateToSelf Delegate
	 * The delegate serves as an event notification system within the Gameplay Ability System.
	 * By binding functions to this delegate, you can perform specific actions whenever a gameplay effect is applied to the actor.
	 * This is particularly useful for implementing custom logic that should occur in response to gameplay effects,
	 * such as updating UI elements, triggering animations, or applying additional effects.
	 */
	// Bind FOnGameplayEffectAppliedDelegate OnGameplayEffectAppliedDelegateToSelf delegate to this function. This function will be called whenever the delegate is broadcasted.
	// We typically like to do this when the actual game starts and not really in constructors as those are fired off quite early.
	// Delegate function to handle effects applied to self
	// Client RPC, this function will be called in the server and executed on the client.
	UFUNCTION(Client, Reliable)
	void ClientEffectAppliedToSelf(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
