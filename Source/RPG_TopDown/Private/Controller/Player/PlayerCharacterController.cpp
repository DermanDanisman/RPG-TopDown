// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Player/PlayerCharacterController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

APlayerCharacterController::APlayerCharacterController()
{
	// Enabling Controller replication
	bReplicates = true;
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	/* The 'check' macro is used to assert that a condition is true in development builds.
	If 'DefaultInputMappingContext' is nullptr or false, this will crash the game with an error message
	indicating where the check failed. It's a debugging aid that helps ensure variables are properly initialized
	before they're used.
	This is particularly useful during development builds, as it ensures that critical pointers and
	resources are correctly initialized before use.
	Here, it's ensuring that 'DefaultInputMappingContext' has been properly assigned before proceeding.
	*/
	check(DefaultInputMappingContext);

	// Get the input subsystem and add the mapping context
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (InputSubsystem)
	{
		InputSubsystem->AddMappingContext(DefaultInputMappingContext, 0 /*Priority*/);
	}

	// Setting how mouse cursor should behave
	SetCursorSettings();
}

void APlayerCharacterController::SetCursorSettings()
{
	// Enable the mouse cursor to be visible.
	bShowMouseCursor = true;
	// Set the default appearance of the mouse cursor to the standard arrow cursor.
	DefaultMouseCursor = EMouseCursor::Default;
	
	// Create an instance of FInputModeGameAndUI to configure the input mode settings.
	FInputModeGameAndUI InputModeData;
	// Set the behavior for locking the mouse to the viewport.
	// EMouseLockMode::DoNotLock means the mouse can move freely outside the game window.
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// Ensure the mouse cursor is visible even when capturing mouse input (e.g., during a drag operation).
	InputModeData.SetHideCursorDuringCapture(false);
	// Apply the input mode settings to the player controller.
	// This sets up the input mode to allow both game and UI interactions with the configured behavior.
	SetInputMode(InputModeData);
}

void APlayerCharacterController::Move(const FInputActionValue& InputActionValue)
{
	
}
