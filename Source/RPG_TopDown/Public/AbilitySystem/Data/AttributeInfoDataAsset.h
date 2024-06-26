// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfoDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FTopDownAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	// Set from C++ only
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayAttribute AttributeGetter;
	
};


/**
 * Here we want to create a struct to store all the information associated with a given attribute.
 * So once an attribute changes, then we can broadcast this struct object up to the widget blueprints who will receive it and update themselves with that information.
 */
UCLASS()
class RPG_TOPDOWN_API UAttributeInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	// We have a function we can call, and it's a public function that we can simply pass in a tag to and receive the attribute info.
	FTopDownAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;

	// Attribute array list in Data Asset, filled in blueprint
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty="AttributeName"))
	TArray<FTopDownAttributeInfo> AttributeInformation;
};
