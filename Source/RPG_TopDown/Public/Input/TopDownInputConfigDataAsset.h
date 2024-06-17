// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "TopDownInputConfigDataAsset.generated.h"

/* Forward Declaring */
class UInputAction;

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
 * 
 */
UCLASS()
class RPG_TOPDOWN_API UTopDownInputConfigDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilityInputActions|TArray")
	TArray<FTopDownInputAction> AbilityInputActions;

	/*
	 * TMap for finding Ability Input Action that has matching tag
	 */
	const UInputAction* FindAbilityInputActionForTagMap(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilityInputActions|TMap")
	TMap<TObjectPtr<UInputAction>, FGameplayTag> AbilityInputActionTags;
};
