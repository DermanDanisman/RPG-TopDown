// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModifierMagnitudeCalculation/MMC_MaxStamina.h"

#include "AbilitySystem/BaseAttributeSet.h"
#include "Interface/Interaction/CombatInterface.h"

UMMC_MaxStamina::UMMC_MaxStamina()
{
	VigorDef.AttributeToCapture = UBaseAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxStamina::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max(Vigor, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetCharacterLevel();

	return 50.f + (Vigor * 2.f) + (PlayerLevel * 10.f);
}
