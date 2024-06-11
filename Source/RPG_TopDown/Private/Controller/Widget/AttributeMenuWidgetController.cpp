// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Widget/AttributeMenuWidgetController.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfoDataAsset.h"
#include "TopDownGameplayTags.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	// Cast the AttributeSet to UBaseAttributeSet to access its methods.
	const UBaseAttributeSet* BaseAttributeSet = CastChecked<UBaseAttributeSet>(AttributeSet);

	// Check if the Data Asset for Attribute Info is valid in blueprint
	check(AttributeInfoDataAsset);

	for (const auto& Tag : AttributeInfoDataAsset.Get()->AttributeInformation)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Tag.AttributeGetter).AddLambda(
		[this, &Tag, &BaseAttributeSet] (const FOnAttributeChangeData& Data)
			{
				FTopDownAttributeInfo Info = AttributeInfoDataAsset->FindAttributeInfoForTag(Tag.AttributeTag);
				Info.AttributeValue = Info.AttributeGetter.GetNumericValue(BaseAttributeSet);
				AttributeInfoDelegate.Broadcast(Info);
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	// Cast the AttributeSet to UBaseAttributeSet to access its methods.
	const UBaseAttributeSet* BaseAttributeSet = CastChecked<UBaseAttributeSet>(AttributeSet);

	// Check if the Data Asset for Attribute Info is valid in blueprint
	check(AttributeInfoDataAsset);

	for (const auto& Tag : AttributeInfoDataAsset.Get()->AttributeInformation)
	{
		FTopDownAttributeInfo Info = AttributeInfoDataAsset->FindAttributeInfoForTag(Tag.AttributeTag);
		Info.AttributeValue = Info.AttributeGetter.GetNumericValue(BaseAttributeSet);
		AttributeInfoDelegate.Broadcast(Info);
	}
}
