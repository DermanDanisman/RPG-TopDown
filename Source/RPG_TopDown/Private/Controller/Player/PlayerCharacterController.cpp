// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Player/PlayerCharacterController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interface/Interaction/HighlightActorInterface.h"
#include "Interface/Camera/CameraMovementInterface.h"

APlayerCharacterController::APlayerCharacterController()
{
	// Enabling Controller replication
	bReplicates = true;
}

void APlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
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

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind input actions to their respective functions
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(InputActionMove, ETriggerEvent::Triggered, this, &APlayerCharacterController::Move);
		EnhancedInputComponent->BindAction(InputActionCameraRotate, ETriggerEvent::Triggered, this, &APlayerCharacterController::RotateCamera);
		EnhancedInputComponent->BindAction(InputActionCameraRotate, ETriggerEvent::Completed, this, &APlayerCharacterController::StopRotateCamera);
		EnhancedInputComponent->BindAction(InputActionCameraZoomInOut, ETriggerEvent::Triggered, this, &APlayerCharacterController::CameraZoomInOut);
	}
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
	// Extracts the 2D input vector (X and Y) from the input action value
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	// Gets the current rotation of the controller
	const FRotator Rotation = GetControlRotation();
	// Creates a rotation that only affects the Yaw (horizontal rotation)
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	// Gets the forward direction vector based on the Yaw rotation
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// Gets the right direction vector based on the Yaw rotation
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Retrieves the pawn that the controller is possessing
	APawn* ControlledPawn = GetPawn<APawn>();
	// If the pawn is valid, apply movement input based on the forward and right direction vectors
	if (ControlledPawn)
	{
		// Adds movement in the forward direction based on the Y input value (forward/backward)
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);

		// Adds movement in the right direction based on the X input value (left/right)
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
	
	/*
	// Extract the 2D input vector (X and Y) from the input action value
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	// Get the pawn the controller is possessing
	APawn* ControlledPawn = GetPawn<APawn>();

	if (ControlledPawn)
	{
		// Create movement vectors based on input
		const FVector ForwardDirection = FVector::ForwardVector;
		const FVector RightDirection = FVector::RightVector;

		// Add movement in the forward and right directions based on input
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}*/
}

void APlayerCharacterController::CameraZoomInOut(const FInputActionValue& InputActionValue)
{
	const float ActionValue = InputActionValue.Get<float>();
	
	APawn* ControlledPawn = GetPawn<APawn>();
	if (ControlledPawn)
	{
		CameraMovementInterface = ControlledPawn;
		CameraMovementInterface->CameraZoom(ActionValue);
	}
}

void APlayerCharacterController::RotateCamera(const FInputActionValue& InputActionValue)
{
	const float ActionValue = InputActionValue.Get<float>();
	
	// Retrieves the pawn that the controller is possessing
	APawn* ControlledPawn = GetPawn<APawn>();
	if (ControlledPawn)
	{
		CameraMovementInterface = ControlledPawn;
		CameraMovementInterface->RotateCamera(ActionValue);
	}
}

void APlayerCharacterController::StopRotateCamera(const FInputActionValue& InputActionValue)
{
	const float ActionValue = InputActionValue.Get<float>();
	
	// Retrieves the pawn that the controller is possessing
	APawn* ControlledPawn = GetPawn<APawn>();
	if (ControlledPawn)
	{
		CameraMovementInterface = ControlledPawn;
		CameraMovementInterface->RotateCamera(ActionValue);
	}
}

void APlayerCharacterController::CursorTrace()
{
	// Declare a hit result to store the outcome of the trace
	FHitResult CursorHitResult;
	// Perform a line trace (ray cast) under the cursor using the visibility channel
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);
	// If the trace did not hit anything, exit the function
	if (!CursorHitResult.bBlockingHit) return;
	
	// Store the actor hit by the previous trace
	LastActor = ThisActor;
	// Store the actor hit by the current trace
	ThisActor = CursorHitResult.GetActor();
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue,
		FString::Printf(TEXT("EnemyCharacter %s: HighlightActor Called"), *CursorHitResult.GetActor()->GetName()));
    
	// If the actor under the cursor has changed since the last frame
	if (ThisActor != LastActor)
	{
		// If the previous actor is valid, unhighlight it
		if (LastActor != nullptr) LastActor->UnHighlightActor();
        
		// If the current actor is valid, highlight it
		if (ThisActor != nullptr) ThisActor->HighlightActor();
	}
}
