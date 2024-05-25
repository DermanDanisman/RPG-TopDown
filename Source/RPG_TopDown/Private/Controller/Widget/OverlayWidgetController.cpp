// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Widget/OverlayWidgetController.h"

#include "AbilitySystem/BaseAttributeSet.h"


// Broadcasts the initial values of the attributes when the widget is first initialized.
void UOverlayWidgetController::BroadcastInitialValues()
{
	// Cast the AttributeSet to UBaseAttributeSet to access its methods.
	const UBaseAttributeSet* BaseAttributeSet = Cast<UBaseAttributeSet>(AttributeSet);
	
	// Broadcast the initial health value.
	OnHealthChanged.Broadcast(BaseAttributeSet->GetHealth());
	// Broadcast the initial max health value.
	OnMaxHealthChanged.Broadcast(BaseAttributeSet->GetMaxHealth());

	// Broadcast the initial mana value.
	OnManaChanged.Broadcast(BaseAttributeSet->GetMana());
	// Broadcast the initial max mana value.
	OnMaxManaChanged.Broadcast(BaseAttributeSet->GetMaxMana());

	// Broadcast the initial stamina value.
	OnStaminaChanged.Broadcast(BaseAttributeSet->GetStamina());
	// Broadcast the initial max stamina value.
	OnMaxStaminaChanged.Broadcast(BaseAttributeSet->GetMaxStamina());
}

// Binds the callbacks to the attribute change delegates.
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// Cast the AttributeSet to UBaseAttributeSet to access its methods.
	const UBaseAttributeSet* BaseAttributeSet = Cast<UBaseAttributeSet>(AttributeSet);

	// UOverlayWidgetController::HealthChanged is this callback for BaseAttributeSet->GetHealthAttribute()
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetStaminaAttribute()).AddUObject(this, &UOverlayWidgetController::StaminaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxStaminaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxStaminaChanged);
}

// Callback function that broadcasts the health change event.
void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

// Callback function that broadcasts the max health change event.
void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

// Callback function that broadcasts the mana change event.
void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

// Callback function that broadcasts the max mana change event.
void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

// Callback function that broadcasts the stamina change event.
void UOverlayWidgetController::StaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnStaminaChanged.Broadcast(Data.NewValue);
}

// Callback function that broadcasts the max stamina change event.
void UOverlayWidgetController::MaxStaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxStaminaChanged.Broadcast(Data.NewValue);
}
