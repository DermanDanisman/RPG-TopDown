// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModifierMagnitudeCalculation/MMC_MaxHealth.h"

#include "AbilitySystem/BaseAttributeSet.h"
#include "Interface/Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	// We define the attribute we want to capture, in this case, Vigor.
	// The GetVigorAttribute function is defined by the ATTRIBUTE_ACCESSORS macro in the BaseAttributeSet.h file.
	VigorDef.AttributeToCapture = UBaseAttributeSet::GetVigorAttribute();
	
	/*
	 * If we're capturing vigor, we have to define whether we're capturing vigor from the target or the source.
	 * Now, in our case for this, we're applying the gameplay effect from the base character to the base character.
	 * So they're both the same, but we're going to specify here.
	 * We want to define the attribute source, and we use an enum for this EGameplayEffectAttributeCaptureSource
	 */
	// We specify that we are capturing the attribute from the target of the effect.
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	/*
	 * When do we want to capture the attribute?
	 * When the gameplay effect spec is created or when the gameplay effect spec is applied?
	 * Well, in our case, those two things happen one after the other.
	 * We create the gameplay effects back, and we apply it immediately within the same function call, so it doesn't really matter.
	 * But later we're going to be creating gameplay effect specs and then not necessarily applying them immediately.
	 * For example, we might create an effect spec and set it on a fireball that flies through the air,
	 * and then eventually that fireball may or may not hit something and then apply that effects back.
 	 * And so the question is if we're capturing attributes in that gameplay effect, do we want to capture them when that gameplay effect spec was first created at that point in time?
	 * Or do we want to capture them when the effect is applied at that point in time?
	 * Snapshotting is capturing the attribute right away.
	 * As soon as the effect spec is created, not Snapshotting is getting the true value at the time of application of the effect.
	 */
	// Snapshotting determines whether the attribute value is captured at the time the effect spec is created
	// or at the time the effect is applied. Here we choose to capture the value at the time of application.
	// bSnapshot is set to false, meaning the attribute's value will be captured at the time the effect is applied, not when the effect spec is created.
	VigorDef.bSnapshot = false;

	// We add this attribute capture definition to the list of attributes that this custom calculation will capture.
	// The attribute capture definition is added to the RelevantAttributesToCapture array, which the gameplay ability system uses to know which attributes to capture for this calculation.
	RelevantAttributesToCapture.Add(VigorDef);
}

// This function calculates the base magnitude of the gameplay effect spec.
float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
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
	
	// Ensure that the Vigor value is non-negative.
	Vigor = FMath::Max<float>(Vigor, 0.f);

	// We use the ICombatInterface to get additional context, such as the level of the character.
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetCharacterLevel();

	// Calculate the final magnitude for max health.
	// This formula can be adjusted based on the game's balance requirements.
	return 50.f + (Vigor * 2.f) + (PlayerLevel * 10.f);
}
