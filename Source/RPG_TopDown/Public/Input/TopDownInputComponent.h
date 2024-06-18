// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "TopDownInputConfigDataAsset.h"
#include "TopDownInputComponent.generated.h"

/**
 * The UTopDownInputComponent class provides a flexible way to bind various input actions to different callback functions using a templated approach.
 * This allows for great flexibility and reuse, as you can bind different functions to different input actions without needing to write redundant code.
 * By using this template function, you can easily set up input bindings for abilities in your game,
 * ensuring that they are correctly triggered when the player interacts with the input system.
 */

UCLASS()
class RPG_TOPDOWN_API UTopDownInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:

	// A function that we can call from our player controller that will bind inputs to callbacks, and I'd like those callbacks to be parameters to a function.
	// So this function is a template function capable of receiving functions or function pointers and whatever that function signature is. It doesn't matter.
	// This is a template, so we can just pass in the function we want and this function will handle it.
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UTopDownInputConfigDataAsset* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

// A template function to bind input actions to callbacks.
// UserClass: The class type that owns the functions.
// PressedFuncType: The type of the function to call when the input is pressed.
// ReleasedFuncType: The type of the function to call when the input is released.
// HeldFuncType: The type of the function to call when the input is held.
template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UTopDownInputComponent::BindAbilityActions(const UTopDownInputConfigDataAsset* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	// Ensure the input configuration is valid
	check(InputConfig);

	// Loop through each ability input action defined in the input configuration
	for (const auto& InputActionStruct : InputConfig->AbilityInputActions)
	{
		// Check if the input action and the input tag are valid
		if (InputActionStruct.InputAction && InputActionStruct.InputTag.IsValid())
		{
			/*
			 * InputConfig: A pointer to a data asset containing the input configuration.
			 * Object: A pointer to the object owning the functions.
			 * PressedFunc: The function to call when the input is pressed.
			 * ReleasedFunc: The function to call when the input is released.
			 * HeldFunc: The function to call when the input is held.
			 */
			
			// If a function for pressed input is provided, bind it to the Started event of the input action
			if (PressedFunc)
			{
				BindAction(InputActionStruct.InputAction, ETriggerEvent::Started, Object, PressedFunc, InputActionStruct.InputTag);
			}

			// If a function for released input is provided, bind it to the Completed event of the input action
			if (ReleasedFunc)
			{
				BindAction(InputActionStruct.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputActionStruct.InputTag);
			}
            
			// If a function for held input is provided, bind it to the Triggered event of the input action
			if (HeldFunc)
			{
				BindAction(InputActionStruct.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputActionStruct.InputTag);
			}
		}
	}
}
