// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Widget/BaseWidgetController.h"
#include "OverlayWidgetController.generated.h"

/**
 * I'd like them to be dynamic multicast because for one, I'd like to assign events to them in Blueprint and in widget blueprint specifically.
 * That's why I want them to be dynamic and multicast because multiple blueprints,
 * multiple widget blueprints may want to bind to these delegates so that they can update.
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, ManaHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class RPG_TOPDOWN_API UOverlayWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadcastInitialValues() override;

	// The "Signature" makes it clear that this is the delegate type and then OnHealthChanged is the delegate itself.
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnManaChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxManaChangedSignature OnMaxManaChanged;
};
