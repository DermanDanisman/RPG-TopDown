// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Player/PlayerCharacterController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "TopDownGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "ActorComponent/CameraMovementComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Input/TopDownInputComponent.h"
#include "Interface/Interaction/HighlightActorInterface.h"
#include "Interface/Camera/CameraMovementInterface.h"
#include "RPG_TopDown/RPG_TopDown.h"
#include "UI/Widget/DamageTextWidgetComponent.h"


APlayerCharacterController::APlayerCharacterController()
{
	// Enabling Controller replication
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void APlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	AutoRun();
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
	UTopDownInputComponent* TopDownInputComponent = CastChecked<UTopDownInputComponent>(InputComponent);
	if (TopDownInputComponent)
	{
		TopDownInputComponent->BindAction(InputActionMove, ETriggerEvent::Triggered, this, &APlayerCharacterController::Move);
		TopDownInputComponent->BindAction(InputActionCameraZoomInOut, ETriggerEvent::Triggered, this, &APlayerCharacterController::CameraZoomInOut);
		TopDownInputComponent->BindAction(InputActionShift, ETriggerEvent::Started, this, &APlayerCharacterController::ShiftPressed);
		TopDownInputComponent->BindAction(InputActionShift, ETriggerEvent::Completed, this, &APlayerCharacterController::ShiftReleased);
		
		TopDownInputComponent->BindAbilityActions(InputConfigDataAsset, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	}
}

void APlayerCharacterController::AutoRun()
{
	/*
	 * Check Allow Client Side Navigation in project settings
	 */
	
	if (!bAutoRunning) return;
	
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedMoveDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

// Performs a line trace (raycast) from the cursor's position to detect what the cursor is pointing at.
// It updates the highlighted actor if the cursor moves over a different actor.
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
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue,
	//FString::Printf(TEXT("EnemyCharacter %s: HighlightActor Called"), *CursorHitResult.GetActor()->GetName()));
    
	// If the actor under the cursor has changed since the last frame
	if (ThisActor != LastActor)
	{
		// If the previous actor is valid, unhighlight it
		if (LastActor != nullptr) LastActor->UnHighlightActor();
		// If the current actor is valid, highlight it
		if (ThisActor != nullptr) ThisActor->HighlightActor();
	}
}

// Configures the cursor to be visible and allows both game and UI interactions.
// The cursor is not locked to the viewport and remains visible during capture operations.
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
		bAutoRunning = false;
		// Adds movement in the forward direction based on the Y input value (forward/backward)
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		// Adds movement in the right direction based on the X input value (left/right)
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

// Adjusts the camera zoom level based on input.
// Checks if the input tag matches the left mouse button (LMB) tag.
// If it does, it sets bTargeting based on whether the player is currently targeting an actor and stops auto-running.
void APlayerCharacterController::CameraZoomInOut(const FInputActionValue& InputActionValue)
{
	const float ActionValue = InputActionValue.Get<float>();

	if (const APawn* ControlledPawn = GetPawn<APawn>())
	{
		CameraMovementInterface = ControlledPawn->GetComponentByClass(UCameraMovementComponent::StaticClass());
		CameraMovementInterface->CameraZoom(ActionValue);
	}
}

// Input Callback for Pressed Action
// Activates the ability associated with the released input tag if it's not the LMB.
// If it is the LMB and the player is targeting, it activates the ability. Otherwise, it handles movement and pathfinding, drawing the path and setting up auto-running.
void APlayerCharacterController::AbilityInputTagPressed(const FGameplayTag InputTag)
{
	// Check if we are targeting something. If not we can move
	if (InputTag.MatchesTagExact(FTopDownGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

// Input Callback for Released Action
// Activates the ability associated with the held input tag if it's not the LMB.
// If it is the LMB and the player is targeting, it activates the ability. Otherwise, it handles continuous movement towards the cursor position.
void APlayerCharacterController::AbilityInputTagReleased(const FGameplayTag InputTag)
{
	// If the input tag is not the LMB, the function directly activates the ability associated with the held input tag.
	if (!InputTag.MatchesTagExact(FTopDownGameplayTags::Get().InputTag_LMB))
	{
		if (GetAbilitySystemComponent())
		{
			GetAbilitySystemComponent()->ActivateAbilityInputTagReleased(InputTag);
		}
		return;
	}

	GetAbilitySystemComponent()->ActivateAbilityInputTagReleased(InputTag);
	
	if (!bTargeting && !bShiftKeyDown)
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if (UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,
				ControlledPawn->GetActorLocation(), CachedMoveDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLocation : NavigationPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLocation, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLocation, 12.f, 12, FColor::Green, false, 5.f);
				}
				if (NavigationPath->PathPoints.Num() > 0)
				{
					CachedMoveDestination = NavigationPath->PathPoints.Last();
					bAutoRunning = true;
				}
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

// Input Callback for Held Action
void APlayerCharacterController::AbilityInputTagHeld(const FGameplayTag InputTag)
{
	// If the input tag is not the LMB, the function directly activates the ability associated with the held input tag.
	if (!InputTag.MatchesTagExact(FTopDownGameplayTags::Get().InputTag_LMB))
	{
		if (GetAbilitySystemComponent())
		{
			GetAbilitySystemComponent()->ActivateAbilityInputTagHeld(InputTag);
		}
		return;
	}

	// If the input tag is LMB and the player is targeting something (bTargeting is true), the ability associated with the held input tag is activated.
	if (bTargeting || bShiftKeyDown)
	{
		if (GetAbilitySystemComponent())
		{
			GetAbilitySystemComponent()->ActivateAbilityInputTagHeld(InputTag);
		}
	}
	else // If the input tag is LMB and the player is not targeting anything (bTargeting is false), the character is moved towards the cursor position.
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		FHitResult HitResult;
		// ECC_GameTraceChannel1 = Navigation
		if (GetHitResultUnderCursor(ECC_Navigation, false, HitResult))
		{
			CachedMoveDestination = HitResult.ImpactPoint;
		}

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedMoveDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

// This function retrieves the ability system component associated with the player’s pawn.
// If the component is not already cached, it uses UAbilitySystemBlueprintLibrary to find and cast it to UBaseAbilitySystemComponent.
UBaseAbilitySystemComponent* APlayerCharacterController::GetAbilitySystemComponent()
{
	if (BaseAbilitySystemComponent == nullptr)
	{
		BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>
		(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return BaseAbilitySystemComponent;
}

void APlayerCharacterController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* Target)
{
	if (IsValid(Target) && DamageTextWidgetComponentClass)
	{
		UDamageTextWidgetComponent* DamageText = NewObject<UDamageTextWidgetComponent>(Target, DamageTextWidgetComponentClass);
		// After creating a component dynamically, we need to register it manually.
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(Target->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount);
	}
}

