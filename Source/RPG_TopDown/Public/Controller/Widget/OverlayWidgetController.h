// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Widget/BaseWidgetController.h"
#include "OverlayWidgetController.generated.h"

/* Forward Declaring */
struct FOnAttributeChangeData;

/**
 * I'd like them to be dynamic multicast because for one, I'd like to assign events to them in Blueprint and in widget blueprint specifically.
 * That's why I want them to be dynamic and multicast because multiple blueprints,
 * multiple widget blueprints may want to bind to these delegates so that they can update.
 */


// Declaration of dynamic multicast delegates with one float parameter.
// These will be used to notify listeners about attribute changes in Blueprints.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChangedSignature, float, NewStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxStaminaChangedSignature, float, NewMaxStamina);

/**
 * UOverlayWidgetController is responsible for managing the UI overlay widget,
 * handling attribute changes, and broadcasting these changes to Blueprint.
 */

UCLASS(BlueprintType, Blueprintable)
class RPG_TOPDOWN_API UOverlayWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	
	// Broadcasts the initial values of the attributes when the widget is first initialized.
	virtual void BroadcastInitialValues() override;

	// Binds the callbacks to the attribute change delegates.
	virtual void BindCallbacksToDependencies() override;

	// Dynamic multicast delegates for broadcasting attribute changes.
	// These properties can be assigned and bound to events in Blueprints.
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnManaChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxManaChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnManaChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxManaChangedSignature OnMaxStaminaChanged;

protected:

	/** Callback functions for specified attribute changes */
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void ManaChanged(const FOnAttributeChangeData& Data) const;
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;
	void StaminaChanged(const FOnAttributeChangeData& Data) const;
	void MaxStaminaChanged(const FOnAttributeChangeData& Data) const;
};
