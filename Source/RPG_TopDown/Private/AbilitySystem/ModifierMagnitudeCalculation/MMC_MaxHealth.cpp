// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModifierMagnitudeCalculation/MMC_MaxHealth.h"

#include "AbilitySystem/BaseAttributeSet.h"
#include "Interface/Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	// We can call UBaseAttributeSet::GetVigorAttribute() because the AttributeAccessors macro we defined in BaseAttributeSet.h.
	// GetVigorAttribute is a static function thanks to that macro.
	VigorDef.AttributeToCapture = UBaseAttributeSet::GetVigorAttribute();
	/*
	 * If we're capturing vigor, we have to define whether we're capturing vigor from the target or the source.
	 * Now, in our case for this, we're applying the gameplay effect from the base character to the base character.
	 * So they're both the same, but we're going to specify here.
	 * We want to define the attribute source, and we use an enum for this EGameplayEffectAttributeCaptureSource
	 */
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	// Snapshotting has to do with timing.
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
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from the source and target as well
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetCharacterLevel();

	return 50.f + (Vigor * 2.f) + (PlayerLevel * 10.f);
}
