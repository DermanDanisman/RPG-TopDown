// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemComponent.generated.h"

// Created Delegate for Widget Controller communication.
DECLARE_MULTICAST_DELEGATE_OneParam(FGameplayEffectAssetTags, const FGameplayTagContainer& /* Asset Tags */);

/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	// Function to bind the gameplay effect delegates, This is also called Ability Actor Info Set.
	void BindOnGameplayEffectAppliedDelegateToSelf();

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

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
	void EffectAppliedToSelf(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
