// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseWidgetController.generated.h"

/* Forward Declaring Classes */
class UAttributeSet;
class UAbilitySystemComponent;

/**
 * Now, BaseWidgetController is going to be responsible for getting any data from the system, from the model in this architecture.
 * That means our Ability System Component, Attribute Set, Character, anything that really matters relating to data.
 * The widget controller will be responsible for getting that data and broadcasting it over to any widgets that have their controller set to it.
 */

/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API UBaseWidgetController : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category="Main Data Classes")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="Main Data Classes")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="Main Data Classes")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="Main Data Classes")
	TObjectPtr<UAttributeSet> AttributeSet;
	
};
