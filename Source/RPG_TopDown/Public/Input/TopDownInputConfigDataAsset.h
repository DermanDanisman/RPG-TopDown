// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "TopDownInputConfigDataAsset.generated.h"

/* Forward Declaration */
class UInputAction;

// A struct that maps input actions to gameplay tags.
USTRUCT(BlueprintType)
struct FTopDownInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * UTopDownInputConfigDataAsset
 * This class holds input action configurations, mapping input actions to gameplay tags.
 */
UCLASS()
class RPG_TOPDOWN_API UTopDownInputConfigDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	// Function to find an input action by its associated gameplay tag
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

	// An array of FTopDownInputAction to hold input configurations.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilityInputActions|TArray")
	TArray<FTopDownInputAction> AbilityInputActions;

	// Another function to find an input action by its associated gameplay tag using a TMap
	const UInputAction* FindAbilityInputActionForTagMap(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilityInputActions|TMap")
	TMap<TObjectPtr<UInputAction>, FGameplayTag> AbilityInputActionTags;
};
