// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Controller/Player/PlayerCharacterController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerState/TopDownPlayerState.h"
#include "RPG_TopDown/RPG_TopDown.h"
#include "UI/HUD/TopDownHUD.h"


APlayerCharacter::APlayerCharacter()
{
	// Disable controller rotation for pitch, yaw, and roll
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Create and configure the Camera Spring Arm
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>("CameraSpringArm");
	CameraSpringArm->SetupAttachment(GetCapsuleComponent());
	CameraSpringArm->bDoCollisionTest = false;
	CameraSpringArm->bInheritYaw = false;
	CameraSpringArm->TargetArmLength = TargetArmLength;
	CameraSpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	CameraSpringArm->bEnableCameraLag = true;
	CameraSpringArm->CameraLagSpeed = 6.f;

	// Create and configure the Camera
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraSpringArm);
	Camera->bUsePawnControlRotation = false;  // Disable camera rotation controlled by the pawn

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;  // Character moves in the direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);  // Set rotation rate for turning
	GetCharacterMovement()->bConstrainToPlane = true;  // Constrain movement to a plane
	GetCharacterMovement()->bSnapToPlaneAtStart = true;  // Snap to the plane at the start

}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetCharacterMovement()->Velocity.Size() < 1.f)
	{
		const FVector2D MousePosPercent = GetMousePositionPercent();
		CameraEdgeScrolling(DeltaSeconds, MousePosPercent);
	}
	else
	{
		ResetCamera();
	}
}

// Called when the character is possessed by a new controller (i.e., when the player takes control of this character)
void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Calling Ability System Init Ability Actor Info for the Server
	InitAbilityActorInfo();
}

// Called when the PlayerState is replicated (i.e., when the PlayerState is updated on clients)
void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Calling Ability System Init Ability Actor Info for the Clients
	InitAbilityActorInfo();
}

void APlayerCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	// Get the player state and player controller
	PlayerPS = GetPlayerState<ATopDownPlayerState>();
	// Check if player state and player controller are valid
	checkf(PlayerPS, TEXT("Player Character State Uninitialized!"));

	if (PlayerPS)
	{
		/*
		 * InitAbilityActorInfo is a function typically used in the context of Unreal Engine's Gameplay Ability System (GAS).
		 * Its primary purpose is to initialize the ability-related information for an actor,
		 * such as setting up the AbilitySystemComponent and the associated AttributeSet.
		 * This function ensures that the actor has all the necessary components and information to use abilities and manage their attributes effectively.
		 */
		/*
		 * Typical Responsibilities of InitAbilityActorInfo
	
		 1. Retrieve and Set Player State and Controller References:
			It obtains references to the player state and player controller, which are essential for managing the actor's state and abilities.
	
		 2. Initialize the Ability System Component:
			It sets up the AbilitySystemComponent, which is responsible for managing abilities and gameplay effects.
	
		 3. Set the Attribute Set:
			It assigns the AttributeSet to manage various attributes like health, mana, strength, etc.
	
		 4. Setup Initial Ability Info:
			It initializes the AbilitySystemComponent with the actor's information, linking it to the player state and the actor itself.
	
		 5. Initialize any UI-related Components:
			It can also initialize any UI components related to the ability system, such as updating the HUD with the initial attribute values.
		 */
		// Initialize the Ability System Component with actor info
		PlayerPS->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerPS, this);
		// Binding FOnGameplayEffectAppliedDelegate OnGameplayEffectAppliedDelegateToSelf delegate.
		Cast<UBaseAbilitySystemComponent>(PlayerPS->GetAbilitySystemComponent())->BindOnGameplayEffectAppliedDelegateToSelf();
		AbilitySystemComponent = PlayerPS->GetAbilitySystemComponent();
		AttributeSet = PlayerPS->GetAttributeSet();

		// Cast the controller to APlayerCharacterController
		PlayerPC = Cast<APlayerCharacterController>(GetController());
        
		if (PlayerPC)
		{
			// Cast the HUD to ATopDownHUD
			if (ATopDownHUD* TopDownHUD = Cast<ATopDownHUD>(PlayerPC->GetHUD()))
			{
				// Initialize the overlay widget with player and ability system info
				TopDownHUD->InitializeOverlayWidget(PlayerPC, PlayerPS, AbilitySystemComponent, AttributeSet);
			}
		}

		// This function initializes the character's default attributes by applying primary and secondary attribute effects to the character.
		InitializeDefaultAttributes();
	}
}

//Highlight the actor by enabling custom depth rendering with a specific stencil value
void APlayerCharacter::HighlightActor()
{
	// Enable custom depth rendering for the mesh
	GetMesh()->SetRenderCustomDepth(true);
	// Set the stencil value for custom depth rendering to red
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_GREEN);
	// Enable custom depth rendering for the weapon mesh
	WeaponMesh->SetRenderCustomDepth(true);
	// Set the stencil value for the weapon mesh to red
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_GREEN);
}


//Unhighlight the actor by disabling custom depth rendering 
void APlayerCharacter::UnHighlightActor()
{
	// Disable custom depth rendering for the mesh
	GetMesh()->SetRenderCustomDepth(false);
	// Disable custom depth rendering for the weapon mesh
	WeaponMesh->SetRenderCustomDepth(false);
}

// Getting current Player Level
int32 APlayerCharacter::GetCharacterLevel()
{
	check(PlayerPS);
	return PlayerPS->GetPlayerLevel();
}

// Camera zoom
void APlayerCharacter::CameraZoom(const float ActionInput)
{
	// Calculate the new arm length based on input and zoom speed
	const float NewArmLength = CameraSpringArm->TargetArmLength + ActionInput * CameraZoomSpeed * GetWorld()->GetDeltaSeconds();

	// Clamp the arm length to the minimum and maximum zoom values
	CameraSpringArm->TargetArmLength = FMath::Clamp(NewArmLength, TargetArmLengthMin, TargetArmLength); 
}

// Camera Pan Movement
void APlayerCharacter::CameraEdgeScrolling(const float DeltaSeconds, const FVector2D& MousePositionPercent)
{
	if (MousePositionPercent.X > ScreenEdgeHigh)
	{
		if (CameraSpringArm->TargetOffset.Y < ViewDistance)
		{
			CameraSpringArm->TargetOffset.Y += PanSpeed * DeltaSeconds;
		}
	}
	else if (MousePositionPercent.X < ScreenEdgeLow)
	{
		if (CameraSpringArm->TargetOffset.Y > -ViewDistance)
		{
			CameraSpringArm->TargetOffset.Y -= PanSpeed * DeltaSeconds;
		}
	}

	if (MousePositionPercent.Y > ScreenEdgeHigh)
	{
		if (CameraSpringArm->TargetOffset.X > -ViewDistance)
		{
			CameraSpringArm->TargetOffset.X -= PanSpeed * DeltaSeconds;
		}
	}
	else if (MousePositionPercent.Y < ScreenEdgeLow)
	{
		if (CameraSpringArm->TargetOffset.X < ViewDistance)
		{
			CameraSpringArm->TargetOffset.X += PanSpeed * DeltaSeconds;
		}
	}
}

// Purpose: Smoothly resets the camera to the default position using linear interpolation.
void APlayerCharacter::ResetCamera() const
{
	CameraSpringArm->TargetOffset =
		UKismetMathLibrary::VInterpTo(CameraSpringArm->TargetOffset,
			FVector(0.f, 0.f, 0.f), UGameplayStatics::GetWorldDeltaSeconds(this), CameraResetInterpSpeed);
}

// Purpose: Retrieves the mouse position as a percentage of the viewport size.
FVector2D APlayerCharacter::GetMousePositionPercent() const
{
	int32 ViewportSizeX, ViewportSizeY;
	UGameplayStatics::GetPlayerController(this, 0)->GetViewportSize(ViewportSizeX, ViewportSizeY);

	float MousePosX, MousePosY;
	UGameplayStatics::GetPlayerController(this, 0)->GetMousePosition(MousePosX, MousePosY);

	return FVector2D(MousePosX / ViewportSizeX, MousePosY / ViewportSizeY);
}



