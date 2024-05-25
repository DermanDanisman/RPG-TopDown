// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Widget/BaseWidgetController.h"


/**
 * Sets the widget controller variables by copying the values from the provided FWidgetControllerVariables struct.
 * This function initializes the references to the core gameplay components.
 *
 * @param WidgetControllerVariables - The struct containing the references to be set.
 */
void UBaseWidgetController::SetWidgetControllerVariables(const FWidgetControllerVariables& WidgetControllerVariables)
{
    // Set the PlayerController reference
    PlayerController = WidgetControllerVariables.PlayerController;
    
    // Set the PlayerState reference
    PlayerState = WidgetControllerVariables.PlayerState;
    
    // Set the AbilitySystemComponent reference
    AbilitySystemComponent = WidgetControllerVariables.AbilitySystemComponent;
    
    // Set the AttributeSet reference
    AttributeSet = WidgetControllerVariables.AttributeSet;
}
