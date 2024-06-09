// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModifierMagnitudeCalculation/MMC_MaxStamina.h"

#include "AbilitySystem/BaseAttributeSet.h"
#include "Interface/Interaction/CombatInterface.h"

UMMC_MaxStamina::UMMC_MaxStamina()
{
	VigorDef.AttributeToCapture = UBaseAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	// bSnapshot is set to false, meaning the attribute's value will be captured at the time the effect is applied, not when the effect spec is created.
	VigorDef.bSnapshot = false;

	// We add this attribute capture definition to the list of attributes that this custom calculation will capture.
	// The attribute capture definition is added to the RelevantAttributesToCapture array, which the gameplay ability system uses to know which attributes to capture for this calculation.
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxStamina::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from the source and target of the effect.
	// Spec.CapturedSourceTags.GetAggregatedTags() retrieves the gameplay tags associated with the source of the gameplay effect. This could be the actor or object applying the effect.
	// Spec.CapturedTargetTags.GetAggregatedTags() retrieves the gameplay tags associated with the target of the gameplay effect. This could be the actor or object receiving the effect.
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Set up parameters for evaluating the captured attribute values.
	// This structure is used to pass the source and target tags to the attribute evaluation functions.
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Variable to store the captured Vigor attribute value.
	float Vigor = 0.f;

	// Get the captured attribute magnitude. This function retrieves the current value of the attribute we defined in VigorDef.
	// The retrieved value is stored in the Vigor variable.
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max(Vigor, 0.f);

	// We use the ICombatInterface to get additional context, such as the level of the character.
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetCharacterLevel();

	// Calculate the final magnitude for max stamina.
	// This formula can be adjusted based on the game's balance requirements.
	return 50.f + (Vigor * 2.f) + (PlayerLevel * 10.f);
}
