// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/TopDownInputConfigDataAsset.h"
#include "InputAction.h"

const UInputAction* UTopDownInputConfigDataAsset::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const FTopDownInputAction& InputActionStruct : AbilityInputActions)
	{
		if (InputActionStruct.InputAction && InputActionStruct.InputTag == InputTag)
		{
			return InputActionStruct.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Ability Input Action for Input Action Tag [%s], on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}
	
	return nullptr;
}

const UInputAction* UTopDownInputConfigDataAsset::FindAbilityInputActionForTagMap(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const auto& InputAction : AbilityInputActionTags)
	{
		if (InputAction.Key && InputAction.Value == InputTag)
		{
			return InputAction.Key;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Ability Input Action for Input Action Tag [%s], on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
