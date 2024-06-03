// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Controller/Widget/BaseWidgetController.h"
#include "OverlayWidgetController.generated.h"

/*
 * Also Called FUIWidgetRow
 * Rather than just broadcasting a gameplay tag up to the widget side of things,
 * I'd like to have some kind of asset that we can use to look up information that we can broadcast to the widgets.
 * For example, we may have an asset such as a data table that can look things up by tag,
 * by gameplay tag and retrieve some kind of information, some kind of struct that we can pass up to the widgets.
 * FGameplayTag: Indicates that this struct is associated with gameplay tags.
 * MessageInfo: Conveys that this struct holds message information related to those tags.
 */

/* Forward Declaring */
class UBaseUserWidget;
struct FOnAttributeChangeData;

USTRUCT(BlueprintType)
struct FGameplayTagMessageInfoRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText MessageToDisplay = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UBaseUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameplayTagMessageInfoRowSignature, const FGameplayTagMessageInfoRow&, GameplayTagMessageInfoRow);

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
	FOnStaminaChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxStaminaChangedSignature OnMaxStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FGameplayTagMessageInfoRowSignature GameplayTagMessageInfoRowDelegate;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Data")
	TObjectPtr<UDataTable> GameplayTagToUIMessageDataTable;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& GameplayTag);

	/** Callback functions for specified attribute changes */
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void ManaChanged(const FOnAttributeChangeData& Data) const;
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;
	void StaminaChanged(const FOnAttributeChangeData& Data) const;
	void MaxStaminaChanged(const FOnAttributeChangeData& Data) const;

private:
	
	// Function to bind attribute change callbacks to the Ability System Component
	void BindVitalAttributeChangeCallbacks(const UBaseAttributeSet* BaseAttributeSet);
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& GameplayTag)
{
	return DataTable->FindRow<T>(GameplayTag.GetTagName(), TEXT(""));
}
