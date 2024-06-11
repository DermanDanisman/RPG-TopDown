// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "UObject/NoExportTypes.h"
#include "BaseWidgetController.generated.h"

/* Forward Declaration */
class UAttributeSet;
class UAbilitySystemComponent;

/**
 * Struct to initialize key variables needed for the widget controller.
 */
USTRUCT(BlueprintType)
struct FWidgetControllerVariables
{
	GENERATED_BODY()

	// Default constructor
	FWidgetControllerVariables() {}

	// Constructor to initialize variables
	FWidgetControllerVariables(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	// Player Controller reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	// Player State reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	// Ability System Component reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	// Attribute Set reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

/**
 * Now, BaseWidgetController is going to be responsible for getting any data from the system, from the model in this architecture.
 * That means our Ability System Component, Attribute Set, Character, anything that really matters relating to data.
 * The widget controller will be responsible for getting that data and broadcasting it over to any widgets that have their controller set to it.
 */

/**
 * BaseWidgetController is responsible for getting data from the game systems (model)
 * and broadcasting it to the UI (view) in an MVC architecture.
 */
UCLASS()
class RPG_TOPDOWN_API UBaseWidgetController : public UObject
{
	GENERATED_BODY()

public:

	// Function to set the widget controller variables
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerVariables(const FWidgetControllerVariables& WidgetControllerVariables);

	// This will be used in child classes so the base function is empty
	virtual void BroadcastInitialValues();

	// A function to bind callbacks to the dependencies
	virtual void BindCallbacksToDependencies();
	
protected:

	/** Main Class References */
	
	UPROPERTY(BlueprintReadOnly, Category="Main Data Classes")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="Main Data Classes")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="Main Data Classes")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="Main Data Classes")
	TObjectPtr<UAttributeSet> AttributeSet;
	
};
