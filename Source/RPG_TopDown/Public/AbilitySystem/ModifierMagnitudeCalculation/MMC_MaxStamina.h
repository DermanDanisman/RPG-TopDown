// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxStamina.generated.h"

/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API UMMC_MaxStamina : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:

	UMMC_MaxStamina();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:

	FGameplayEffectAttributeCaptureDefinition VigorDef;
	
};
