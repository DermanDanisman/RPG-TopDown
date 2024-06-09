// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModifierMagnitudeCalculation/MMC_MaxMana.h"

#include "AbilitySystem/BaseAttributeSet.h"
#include "Interface/Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UBaseAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	// bSnapshot is set to false, meaning the attribute's value will be captured at the time the effect is applied, not when the effect spec is created.
	IntelligenceDef.bSnapshot = false;
	
	// We add this attribute capture definition to the list of attributes that this custom calculation will capture.
	// The attribute capture definition is added to the RelevantAttributesToCapture array, which the gameplay ability system uses to know which attributes to capture for this calculation.
	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
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

	// Variable to store the captured Intelligence attribute value.
	float Intelligence = 0.f;

	// Get the captured attribute magnitude. This function retrieves the current value of the attribute we defined in IntelligenceDef.
	// The retrieved value is stored in the Vigor variable.
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);
	Intelligence = FMath::Max<float>(Intelligence, 0.f);

	// We use the ICombatInterface to get additional context, such as the level of the character.
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetCharacterLevel();

	// Calculate the final magnitude for max mana.
	// This formula can be adjusted based on the game's balance requirements.
	return 25.f + (Intelligence * 5.f) + (PlayerLevel * 5.f);
}
