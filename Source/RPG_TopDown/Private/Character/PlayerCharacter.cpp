// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RPG_TopDown/RPG_TopDown.h"


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
	CameraSpringArm->TargetArmLength = 1000.f;
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

void APlayerCharacter::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_GREEN);
	WeaponMesh->SetRenderCustomDepth(true);
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_GREEN);
	
	// TODO: Remove Later!
	FString ActorName;
	GetName(ActorName);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("EnemyCharacter %s: HighlightActor Called"), *ActorName));
}

void APlayerCharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	WeaponMesh->SetRenderCustomDepth(false);
	
	// TODO: Remove Later!
	FString ActorName;
	GetName(ActorName);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("EnemyCharacter %s: UnHighlightActor Called"), *ActorName));
}
