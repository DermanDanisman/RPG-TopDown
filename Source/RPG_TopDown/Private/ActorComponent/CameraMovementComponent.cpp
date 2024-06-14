// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/CameraMovementComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UCameraMovementComponent::UCameraMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UCameraMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UCameraMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!CameraSpringArm) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && OwnerCharacter->GetCharacterMovement()->Velocity.Size() < 1.f)
	{
		FVector2D MousePosPercent = GetMousePositionPercent();
		UpdateCameraOffset(DeltaTime, MousePosPercent);
	}
	else
	{
		ResetCamera();
	}
}

// Purpose: Adjusts the camera offset based on the mouse position.
void UCameraMovementComponent::UpdateCameraOffset(const float DeltaSeconds, const FVector2D& MousePositionPercent) const
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
void UCameraMovementComponent::ResetCamera() const
{
	CameraSpringArm->TargetOffset =
		UKismetMathLibrary::VInterpTo(CameraSpringArm->TargetOffset,
			FVector(0.f, 0.f, 0.f), UGameplayStatics::GetWorldDeltaSeconds(this), CameraResetInterpSpeed);
}

// Purpose: Retrieves the mouse position as a percentage of the viewport size.
FVector2D UCameraMovementComponent::GetMousePositionPercent() const
{
	int32 ViewportSizeX, ViewportSizeY;
	UGameplayStatics::GetPlayerController(this, 0)->GetViewportSize(ViewportSizeX, ViewportSizeY);

	float MousePosX, MousePosY;
	UGameplayStatics::GetPlayerController(this, 0)->GetMousePosition(MousePosX, MousePosY);

	return FVector2D(MousePosX / ViewportSizeX, MousePosY / ViewportSizeY);
}

// Implements zoom functionality from the Camera Movement Interface
void UCameraMovementComponent::CameraZoom(const float ActionInput)
{
	// Calculate the new arm length based on input and zoom speed
	const float NewArmLength = CameraSpringArm->TargetArmLength + ActionInput * CameraZoomSpeed * GetWorld()->GetDeltaSeconds();

	// Clamp the arm length to the minimum and maximum zoom values
	CameraSpringArm->TargetArmLength = FMath::Clamp(NewArmLength, TargetArmLengthMin, TargetArmLength); 
}


