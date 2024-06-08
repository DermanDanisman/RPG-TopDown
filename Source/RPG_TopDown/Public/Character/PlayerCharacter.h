// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interface/Camera/CameraMovementInterface.h"
#include "Interface/Interaction/HighlightActorInterface.h"
#include "PlayerCharacter.generated.h"


/* Forward Declaring Classes */
class UCameraComponent;
class USpringArmComponent;
class ATopDownPlayerState;
class APlayerCharacterController;

/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API APlayerCharacter : public ABaseCharacter, public IHighlightActorInterface, public ICameraMovementInterface
{
	GENERATED_BODY()

public:

	APlayerCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Highlight Actor Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	/** Combat Interface */
	virtual int32 GetCharacterLevel() override;
	
	/** Camera Movement Interface */
	virtual void CameraZoom(float ActionInput) override;
	virtual void RotateCamera(float ActionInput) override;

	USpringArmComponent* GetCameraSprintArm() { return CameraSpringArm; }

protected:

	/** Game Ability System */
	virtual void InitAbilityActorInfo() override;
	
private:

	/** Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = true), Category="Camera")
	TObjectPtr<USpringArmComponent> CameraSpringArm;
	UPROPERTY(EditAnywhere, Category="Camera")
	float TargetArmLength = 1000.f;
	UPROPERTY(EditAnywhere, Category="Camera")
	float TargetArmLengthMin = 250.f;
	// Camera rotation speed
	UPROPERTY(EditAnywhere, Category="Camera")
	float CameraRotationSpeed = 100.f;
	// Current yaw rotation of the spring arm
	float CurrentYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = true), Category="Camera")
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(EditAnywhere, Category="Camera")
	float CameraZoomSpeed = 750.f;

	/** Game Ability System */
	UPROPERTY(EditAnywhere, Category="Game Ability System")
	TObjectPtr<ATopDownPlayerState> PlayerPS;
	UPROPERTY(EditAnywhere, Category="Game Ability System")
	TObjectPtr<APlayerCharacterController> PlayerPC;
};
