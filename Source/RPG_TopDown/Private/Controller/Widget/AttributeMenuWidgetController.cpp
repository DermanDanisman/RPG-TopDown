// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Widget/AttributeMenuWidgetController.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfoDataAsset.h"


void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	// Cast the AttributeSet to UBaseAttributeSet to access its methods.
	const UBaseAttributeSet* BaseAttributeSet = CastChecked<UBaseAttributeSet>(AttributeSet);

	// Check if the Data Asset for Attribute Info is valid in blueprint
	check(AttributeInfoDataAsset);

	// Iterate over each attribute tag in the data asset and bind its change delegate.
	for (const FTopDownAttributeInfo& Tag : AttributeInfoDataAsset.Get()->AttributeInformation)
	{
		// Add a lambda function to the ability system component's attribute change delegate.
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Tag.AttributeGetter).AddLambda(
		[this, &Tag, BaseAttributeSet] (const FOnAttributeChangeData& Data)
			{
				// Call the function to broadcast attribute info whenever an attribute changes.
				BroadcastAttributeInfo(BaseAttributeSet, Tag);
			}
		);
	}
}

// Broadcasts the initial values of attributes to the UI.
void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	// Cast the AttributeSet to UBaseAttributeSet to access its methods.
	const UBaseAttributeSet* BaseAttributeSet = CastChecked<UBaseAttributeSet>(AttributeSet);

	// Check if the Data Asset for Attribute Info is valid in blueprint
	check(AttributeInfoDataAsset);

	// Iterate over each attribute tag in the data asset and broadcast its info.
	for (const FTopDownAttributeInfo& Tag : AttributeInfoDataAsset.Get()->AttributeInformation)
	{
		BroadcastAttributeInfo(BaseAttributeSet, Tag);
	}
}

// Helper function to broadcast attribute info to UI elements.
void UAttributeMenuWidgetController::BroadcastAttributeInfo(const UAttributeSet* AS, const FTopDownAttributeInfo& Tag) const
{
	// Find the attribute info for the given tag from the data asset.
	FTopDownAttributeInfo Info = AttributeInfoDataAsset->FindAttributeInfoForTag(Tag.AttributeTag);
	// Set the attribute value from the attribute set.
	Info.AttributeValue = Info.AttributeGetter.GetNumericValue(AttributeSet);
	// Broadcast the updated attribute info.
	AttributeInfoDelegate.Broadcast(Info);
}
