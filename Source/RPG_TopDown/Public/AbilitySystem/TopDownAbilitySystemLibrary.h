// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Widget/AttributeMenuWidgetController.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/HUD/TopDownHUD.h"
#include "TopDownAbilitySystemLibrary.generated.h"

/* Forward Declaration */
class UOverlayWidgetController;

/**
 * Purpose: The UBlueprintFunctionLibrary class is used to define static utility functions that can be called from Blueprints.
 * These functions are globally accessible in Blueprints and do not require an instance of the class to be called.
 *
 * Static Functions: Functions in a UBlueprintFunctionLibrary are static because:
 * No Instance Required: They do not need an instance of the class to operate. Instead, they can be called directly from the class.
 * Utility Purpose: They are often utility or helper functions that perform common tasks or calculations which do not depend on the state of an object.
 */

/**
 * Now, with Blueprint function libraries, we usually make these static functions.
 * In fact, we always make them static functions because we don't plan on creating a instance of this class.
 */
UCLASS()
class RPG_TOPDOWN_API UTopDownAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/**
	 * Now here's the thing. This is a static function and if we're going to trace our way all the way up to the HUD,
	 * somehow we need to trace up to the HUD using an object that exists. But a static function cannot access any objects that exist in the world.
	 * Why?
	 * Because the class itself that the static function belongs to, may not exist in the world.
	 * The static function can be called directly, and this is the reason why many function libraries in the engine require a world context object.
	 * It needs a reference that it can use to trace its way up somehow, some way to something in the world that we want to affect.
	 * So our get overlay widget controller class needs a world context object, some kind of object that we can use to have context for the world that we're in.
	 */

	// Gets Overlay Widget Controller
	UFUNCTION(BlueprintPure, Category="TopDownAbilitySystemLibrary|Widget Controller")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	// Gets Attribute Menu Widget Controller
	UFUNCTION(BlueprintPure, Category="TopDownAbilitySystemLibrary|Widget Controller")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
};
