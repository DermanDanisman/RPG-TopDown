// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

	/*
     * Now Modifier Magnitude Calculation can capture attributes. They have access to all the stuff in the effect spec.
	 * That's why they're so powerful. But they also have the ability to capture attributes.
	 */

public:

	UMMC_MaxHealth();

	// We get the gameplay effect spec and that's going to be the effect spec of whatever gameplay effect that has a modifier using this custom calculation.
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:

	// It's a struct that defines what attribute we're going to capture.
	FGameplayEffectAttributeCaptureDefinition VigorDef;
	
};
