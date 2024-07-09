// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecutionCalculation/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "TopDownGameplayTags.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Interface/Interaction/CombatInterface.h"

struct TopDownDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Evasion);

	TopDownDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, MagicResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Evasion, Target, false);
	}
};

static const TopDownDamageStatics& DamageStatics()
{
	static TopDownDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().EvasionDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatarActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	const AActor* TargetAvatarActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec GameplayEffectSpec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = GameplayEffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = GameplayEffectSpec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Get Damage Set by Caller Magnitude
	float Damage = GameplayEffectSpec.GetSetByCallerMagnitude(FTopDownGameplayTags::Get().Damage);

	// Capture Evade on Target, and determine if there was a successful Evasion.
	float TargetEvasion = 0.f;
	// We are getting the evasion value from the target
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().EvasionDef, EvaluationParameters, TargetEvasion);
	TargetEvasion = FMath::Max<float>(TargetEvasion, 0.f);

	const bool bEvaded = FMath::FRandRange(UE_SMALL_NUMBER, 100.f) <= TargetEvasion;
	// if Target evades the attack, zero damage.
	Damage = bEvaded ? Damage = 0.f : Damage;

	// Armor penetration ignores a percentage of the target's armor
	float TargetArmor = 0.f;
	// We are getting the armor value from the target
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	// Armor penetration ignores a percentage of the target's magic resistance
	float TargetMagicResistance = 0.f;
	// We are getting the magic resistance value from the target
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MagicResistanceDef, EvaluationParameters, TargetMagicResistance);
	TargetMagicResistance = FMath::Max<float>(TargetMagicResistance, 0.f);
	
	float SourceArmorPenetration = 0.f;
	// We are getting the armor penetration value from the source
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	const float EffectiveArmor = TargetArmor *= (100 - SourceArmorPenetration * 0.25f) / 100.f;
	Damage *= (100 - EffectiveArmor) / 100.f;

	ICombatInterface* CombatInterfaceSource = Cast<ICombatInterface>(SourceAvatarActor);
	if (CombatInterfaceSource)
	{
		if (CombatInterfaceSource->GetCharacterClass() == ECharacterClass::Sorcerer)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Source is Sorcerer")));
		}
	}
	
	const FGameplayModifierEvaluatedData EvaluatedData(UBaseAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
