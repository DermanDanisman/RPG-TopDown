// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TopDownAbilitySystemGlobals.h"

#include "TopDownCustomAbilityTypes.h"

FGameplayEffectContext* UTopDownAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FTopDownGameplayEffectContext();
}
