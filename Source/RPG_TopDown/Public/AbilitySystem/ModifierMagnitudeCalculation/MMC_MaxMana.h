// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/**
 * This class is derived from UGameplayModMagnitudeCalculation, making it a custom class for calculating the magnitude of a gameplay effect modifier.
 * This is useful when the default calculation methods are not sufficient for the game's needs.
 * Now Modifier Magnitude Calculation can capture attributes. They have access to all the stuff in the effect spec.
 * That's why they're so powerful. But they also have the ability to capture attributes.
 */
UCLASS()
class RPG_TOPDOWN_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
    UMMC_MaxMana();

	// We get the gameplay effect spec and that's going to be the effect spec of whatever gameplay effect that has a modifier using this custom calculation.
	// This function calculates the base magnitude of the gameplay effect spec.
	// It overrides the virtual function from UGameplayModMagnitudeCalculation.
    virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:

	// This struct defines which attribute we are capturing for our custom calculation.
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
};
