// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "TopDownAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API UTopDownAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	/**
	 * The primary purpose of AllocGameplayEffectContext is to provide a way to create and initialize a custom FGameplayEffectContext.
	 * This is useful in situations where you need to extend the default context with additional data,
	 * such as custom hit results, critical hit information, or any other gameplay-specific data.
	 * @return 
	 */
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};
