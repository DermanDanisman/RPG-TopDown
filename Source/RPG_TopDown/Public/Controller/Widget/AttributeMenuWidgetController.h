// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Widget/BaseWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UBaseAttributeSet;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeInfoChangedSignature, float, NewValue);

/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API UAttributeMenuWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:

	virtual void BindCallbacksToDependencies() override;

	virtual void BroadcastInitialValues() override;

	// Dynamic multicast delegates for broadcasting attribute changes.

	/*
	 * Primary Attributes
	 */
	// Strength
	FOnAttributeInfoChangedSignature OnStrengthChanged;
	// Dexterity
	FOnAttributeInfoChangedSignature OnDexterityChanged;
	// Intelligence
	FOnAttributeInfoChangedSignature OnIntelligenceChanged;
	// Resilience
	FOnAttributeInfoChangedSignature OnResilienceChanged;
	// Vigor
	FOnAttributeInfoChangedSignature OnVigorChanged;

	/*
	 * Secondary Attributes
	 */
	// Attack Power
	FOnAttributeInfoChangedSignature OnAttackPowerChanged;
	// Spell Power
	FOnAttributeInfoChangedSignature OnSpellPowerChanged;
	// Armor
	FOnAttributeInfoChangedSignature OnArmorChanged;
	// Magic Resistance
	FOnAttributeInfoChangedSignature OnMagicResistanceChanged;
	// Armor Penetration
	FOnAttributeInfoChangedSignature OnArmorPenetrationChanged;
	// Critical Hit Chance
	FOnAttributeInfoChangedSignature OnCriticalHitChanceChanged;
	// Critical Hit Damage
	FOnAttributeInfoChangedSignature OnCriticalHitDamageChanged;
	// Critical Hit Resistance
	FOnAttributeInfoChangedSignature OnCriticalHitResistanceChanged;
	// Evasion
	FOnAttributeInfoChangedSignature OnEvasionChanged;
	// Movement Speed
	FOnAttributeInfoChangedSignature OnMovementSpeedChanged;
	// Health Regeneration
	FOnAttributeInfoChangedSignature OnHealthRegenerationChanged;
	// Mana Regeneration
	FOnAttributeInfoChangedSignature OnManaRegenerationChanged;
	// Stamina Regeneration
	FOnAttributeInfoChangedSignature OnStaminaRegenerationChanged;
	// Max Health
	FOnAttributeInfoChangedSignature OnMaxHealthChanged;
	// Max Mana
	FOnAttributeInfoChangedSignature OnMaxManaChanged;
	// Max Stamina
	FOnAttributeInfoChangedSignature OnMaxStaminaChanged;


private:

	void BindPrimaryAttributeChangeCallbacks(const UBaseAttributeSet* BaseAttributeSet);

	void BindSecondaryAttributeChangeCallbacks(const UBaseAttributeSet* BaseAttributeSet);
};

