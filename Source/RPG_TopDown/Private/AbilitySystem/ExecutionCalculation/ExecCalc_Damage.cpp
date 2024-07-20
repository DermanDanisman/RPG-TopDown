// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecutionCalculation/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "TopDownCustomAbilityTypes.h"
#include "TopDownGameplayTags.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/TopDownAbilitySystemLibrary.h"
#include "Interface/Interaction/CombatInterface.h"

struct TopDownDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Evasion);

	TopDownDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, MagicResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitResistance, Target, false);
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
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().EvasionDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatarActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatarActor);
	const UCharacterClassInfoDataAsset* SourceCharacterClassInfoDataAsset = UTopDownAbilitySystemLibrary::GetCharacterClassInfoDataAsset(SourceAvatarActor);
	
	AActor* TargetAvatarActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatarActor);
	const UCharacterClassInfoDataAsset* TargetCharacterClassInfoDataAsset = UTopDownAbilitySystemLibrary::GetCharacterClassInfoDataAsset(TargetAvatarActor);
	
	const FGameplayEffectSpec GameplayEffectSpec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle GameplayEffectContextHandle = GameplayEffectSpec.GetContext();

	const FGameplayTagContainer* SourceTags = GameplayEffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = GameplayEffectSpec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Get Damage Set by Caller Magnitude
	float Damage = GameplayEffectSpec.GetSetByCallerMagnitude(FTopDownGameplayTags::Get().Damage);

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
	
	float TargetBlockChance = 0.f;
	// We are getting the block chance value from the target
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	SourceArmorPenetration = FMath::Max<float>(TargetBlockChance, 0.f);

	float SourceCriticalHitChance = 0.f;
	// We are getting the critical hit chance value from the source
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	float SourceCriticalHitDamage = 0.f;
	// We are getting the critical hit damage value from the source
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	float TargetCriticalHitResistance = 0.f;
	// We are getting the critical hit damage value from the source
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

	// Capture Evade on Target, and determine if there was a successful Evasion.
	float TargetEvasion = 0.f;
	// We are getting the evasion value from the target
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().EvasionDef, EvaluationParameters, TargetEvasion);
	TargetEvasion = FMath::Max<float>(TargetEvasion, 0.f);

	const bool bEvaded = FMath::FRandRange(UE_SMALL_NUMBER, 100.f) <= TargetEvasion;
	UTopDownAbilitySystemLibrary::SetIsEvaded(GameplayEffectContextHandle, bEvaded);
	// if Target evades the attack, zero damage.
	Damage = bEvaded ? Damage = 0.f : Damage;
	
	const FRealCurve* ArmorPenetrationCurve = SourceCharacterClassInfoDataAsset->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetCharacterLevel());

	// Target Armor after Armor Penetration applied.
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	
	const FRealCurve* EffectiveArmorCurve = TargetCharacterClassInfoDataAsset->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetCharacterLevel());

	// Armor ignores a percentage of incoming Damage
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;
	
	const bool bBlocked = FMath::FRandRange(UE_SMALL_NUMBER, 100.f) < TargetBlockChance;
	UTopDownAbilitySystemLibrary::SetIsBlockedHit(GameplayEffectContextHandle, bBlocked);
	// If Block, halve the damage.	
	Damage = bBlocked ? Damage / 2.f : Damage;
	
	const FRealCurve* CriticalHitResistanceCurve = TargetCharacterClassInfoDataAsset->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetCharacterLevel());

	// Critical Hit Resistance reduces Critical Hit Chance by a certain percentage
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHitChance = FMath::FRandRange(UE_SMALL_NUMBER, 100.f) <= EffectiveCriticalHitChance;
	UTopDownAbilitySystemLibrary::SetIsCriticalHit(GameplayEffectContextHandle, bCriticalHitChance);

	// Double damage plus a bonus if critical hit
	Damage = bCriticalHitChance ? 2.f * Damage + SourceCriticalHitDamage : Damage;
	
	const FGameplayModifierEvaluatedData EvaluatedData(UBaseAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
