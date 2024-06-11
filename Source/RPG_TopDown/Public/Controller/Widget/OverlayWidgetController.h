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

/* Forward Declaration */
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
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
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FGameplayTagMessageInfoRowSignature GameplayTagMessageInfoRowDelegate;

protected:

	/** GameplayTags Message Tags */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Data")
	TObjectPtr<UDataTable> GameplayTagToUIMessageDataTable;
	// A versatile template function that gets rows from any type of data table.
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& GameplayTag);

private:
	
	// Function to bind attribute change callbacks to the Ability System Component
	void BindVitalAttributeChangeCallbacks(const UBaseAttributeSet* BaseAttributeSet);
	// Function to bind GameplayEffectAssetTags to a lambda function
	void BindGameplayEffectTagMessages();
};

/*
 * Template functions are a feature in C++ that allow functions to operate with generic types.
 * This is achieved through the use of templates, which provide a way to write a single function
 * (or class) to work with different data types without rewriting the entire function (or class) for each type.
 *
 * What is a Template Function?
 * A template function is a function that can operate on data of any type.
 * The type is specified when the function is called.
 * The syntax involves the template keyword followed by template parameters enclosed in angle brackets <>.
 * 
 * Why Use Template Functions?
 * Reusability: Write a function once and use it with different data types.
 * Type Safety: Provides compile-time type checking, ensuring that only valid types are used.
 * Maintainability: Easier to maintain since the logic for different data types is encapsulated in a single function definition.
 *
 * When to Use Template Functions
 * Generic Operations: When you need a function to perform operations that are independent of the data type.
 * Data Structures: Implementing data structures like linked lists, stacks, queues, trees, etc., that should work with any data type.
 * Code Reusability: When you find yourself writing the same function logic for multiple data types, it's a good candidate for templating.
 */
// It finds a row in the data table that matches the given gameplay tag name, and returns a pointer to that row cast to type T.
template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& GameplayTag)
{
	return DataTable->FindRow<T>(GameplayTag.GetTagName(), TEXT(""));
}
