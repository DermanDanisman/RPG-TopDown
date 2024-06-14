// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/TopDownHUD.h"

#include "Blueprint/UserWidget.h"
#include "Controller/Widget/AttributeMenuWidgetController.h"
#include "Controller/Widget/OverlayWidgetController.h"
#include "UI/Widget/BaseUserWidget.h"

// Getter for OverlayWidgetController
UOverlayWidgetController* ATopDownHUD::GetOverlayWidgetController(
	const FWidgetControllerVariables& WidgetControllerVariables)
{
	// Check if OverlayWidgetController is null (i.e., it hasn't been created yet)
	if (OverlayWidgetController == nullptr)
	{
		// Create a new UOverlayWidgetController object, using OverlayWidgetControllerClass as the class type
		// 'this' specifies that the current instance of ATopDownHUD will be the outer object
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		
		// Initialize the widget controller with the provided variables
		OverlayWidgetController->SetWidgetControllerVariables(WidgetControllerVariables);
		
		// Return the newly created and initialized widget controller
		return OverlayWidgetController;
	}
	
	// If OverlayWidgetController already exists, return it
	return OverlayWidgetController;
}

// Getter for AttributeMenuWidgetController
UAttributeMenuWidgetController* ATopDownHUD::GetAttributeMenuWidgetController(const FWidgetControllerVariables& WidgetControllerVariables)
{
	// Check if AttributeMenuWidgetController is null (i.e., it hasn't been created yet)
	if (AttributeMenuWidgetController == nullptr)
	{
		// Create a new UAttributeMenuWidgetController object, using AttributeMenuWidgetControllerClass as the class type
		// 'this' specifies that the current instance of ATopDownHUD will be the outer object
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);

		// Initialize the widget controller with the provided variables
		AttributeMenuWidgetController->SetWidgetControllerVariables(WidgetControllerVariables);
		
		// Return the newly created and initialized widget controller
		return AttributeMenuWidgetController;
	}
	
	// If AttributeMenuWidgetController already exists, return it
	return AttributeMenuWidgetController;
}

// Initialize OverlayWidget
void ATopDownHUD::InitializeOverlayWidget(APlayerController* PC, APlayerState* PS,
                                          UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// Ensure that OverlayWidgetClass is not null, otherwise, display an error
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_TopDownHUD"));
	// Ensure that OverlayWidgetControllerClass is not null, otherwise, display an error
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_TopDownHUD"));
	
	// Create the widget using the specified OverlayWidgetClass
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	// Cast the created widget to UBaseUserWidget and store it in OverlayWidget
	OverlayWidget = Cast<UBaseUserWidget>(Widget);

	// Initialize FWidgetControllerVariables with provided parameters
	const FWidgetControllerVariables WidgetControllerVariables(PC, PS, ASC, AS);
	// Get the OverlayWidgetController using the initialized variables
	UOverlayWidgetController* OverlayWC = GetOverlayWidgetController(WidgetControllerVariables);
	
	// Set the widget controller for the OverlayWidget
	OverlayWidget->SetWidgetController(OverlayWC);
	OverlayWidgetController->BroadcastInitialValues();
	OverlayWidgetController->BindCallbacksToDependencies();
	
	// Add the widget to the viewport, making it visible
	Widget->AddToViewport();
}

