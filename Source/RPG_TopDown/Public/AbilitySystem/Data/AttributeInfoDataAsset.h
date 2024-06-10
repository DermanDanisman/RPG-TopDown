// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;

	
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty="{AttributeName}"))
	TArray<FTopDownAttributeInfo> AttributeInformation;
};
