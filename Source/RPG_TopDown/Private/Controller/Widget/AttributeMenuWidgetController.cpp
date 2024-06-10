// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Widget/AttributeMenuWidgetController.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	// Cast the AttributeSet to UBaseAttributeSet to access its methods.
	const UBaseAttributeSet* BaseAttributeSet = Cast<UBaseAttributeSet>(AttributeSet);
	
	// Function to bind Primary Attributes change callbacks to the Ability System Component
	BindPrimaryAttributeChangeCallbacks(BaseAttributeSet);

	// Function to bind Secondary Attributes change callbacks to the Ability System Component
	BindSecondaryAttributeChangeCallbacks(BaseAttributeSet);
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	// Cast the AttributeSet to UBaseAttributeSet to access its methods.
	const UBaseAttributeSet* BaseAttributeSet = Cast<UBaseAttributeSet>(AttributeSet);
	
	// Broadcast the initial strength value.
	OnStrengthChanged.Broadcast(BaseAttributeSet->GetStrength());
	// Broadcast the initial dexterity value.
	OnDexterityChanged.Broadcast(BaseAttributeSet->GetDexterity());
	// Broadcast the initial intelligence value.
	OnIntelligenceChanged.Broadcast(BaseAttributeSet->GetIntelligence());
	// Broadcast the initial resilience value.
	OnResilienceChanged.Broadcast(BaseAttributeSet->GetResilience());
	// Broadcast the initial vigor value.
	OnVigorChanged.Broadcast(BaseAttributeSet->GetVigor());
	
}

void UAttributeMenuWidgetController::BindPrimaryAttributeChangeCallbacks(const UBaseAttributeSet* BaseAttributeSet)
{
	// Bind callbacks for Strength
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetStrengthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnStrengthChanged.Broadcast(Data.NewValue);
		}
	);

	// Bind callbacks for Dexterity
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetDexterityAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnDexterityChanged.Broadcast(Data.NewValue);
		}
	);

	// Bind callbacks for Intelligence
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetIntelligenceAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnIntelligenceChanged.Broadcast(Data.NewValue);
		}
	);

	// Bind callbacks for Resilience
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetResilienceAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnResilienceChanged.Broadcast(Data.NewValue);
		}
	);

	// Bind callbacks for Vigor
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetVigorAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnVigorChanged.Broadcast(Data.NewValue);
		}
	);
}

void UAttributeMenuWidgetController::BindSecondaryAttributeChangeCallbacks(const UBaseAttributeSet* BaseAttributeSet)
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetAttackPowerAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnAttackPowerChanged.Broadcast(Data.NewValue);		
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetSpellPowerAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnSpellPowerChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetArmorAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnArmorChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMagicResistanceAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMagicResistanceChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetArmorPenetrationAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnArmorPenetrationChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetCriticalHitChanceAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnCriticalHitChanceChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetCriticalHitDamageAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnCriticalHitDamageChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetCriticalHitResistanceAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnCriticalHitResistanceChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetEvasionAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnEvasionChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMovementSpeedAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMovementSpeedChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetHealthRegenerationAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthRegenerationChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetManaRegenerationAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaRegenerationChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetStaminaRegenerationAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnStaminaRegenerationChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxStaminaChanged.Broadcast(Data.NewValue);
		}
	);
}
