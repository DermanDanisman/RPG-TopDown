// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Widget/BaseWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"


/* Forward Declaration */
class UBaseAttributeSet;
struct FTopDownAttributeInfo;
class UAttributeInfoDataAsset;

// Dynamic multicast delegates for broadcasting attribute changes.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FTopDownAttributeInfo&, Info);

/**
 * The UAttributeMenuWidgetController class is responsible for managing and updating the UI elements related to character attributes in the game.
 * It uses dynamic multicast delegates to broadcast changes in attribute values, ensuring the UI reflects the most current state of the character's attributes.
 */
UCLASS(BlueprintType, Blueprintable)
class RPG_TOPDOWN_API UAttributeMenuWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:

	// Function to bind attribute change callbacks to dependencies.
	virtual void BindCallbacksToDependencies() override;

	// Function to broadcast initial attribute values.
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() override;

	// Dynamic multicast delegates for broadcasting attribute changes. Assigned this in Blueprint.
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;


protected:

	// Data asset that contains attribute info. Such as Name, Description, Value, etc.
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfoDataAsset> AttributeInfoDataAsset;

};

