// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interface/Camera/CameraMovementInterface.h"
#include "Interface/Interaction/HighlightActorInterface.h"
#include "PlayerCharacter.generated.h"


/* Forward Declaration */
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

	virtual void Tick(float DeltaSeconds) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Highlight Actor Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	/** Combat Interface */
	virtual int32 GetCharacterLevel() override;
	
	/** Camera Movement Interface */
	virtual void CameraZoom(float ActionInput) override;
	virtual void CameraEdgeScrolling(float DeltaSeconds, const FVector2D& MousePositionPercent) override;

	/** Getter Functions */
	FORCEINLINE USpringArmComponent* GetCameraSpringArm() { return CameraSpringArm; }

protected:

	/** Game Ability System */
	virtual void InitAbilityActorInfo() override;
	
private:

	/** Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> CameraSpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = true) ,Category="Camera Properties")
	float TargetArmLength = 1000.f;
	UPROPERTY(EditAnywhere, Category="Camera Properties")
	float TargetArmLengthMin = 250.f;
	UPROPERTY(EditAnywhere, Category="Camera Properties")
	float CameraZoomSpeed = 750.f;
	
	/** Camera Movement */
	FVector2D GetMousePositionPercent() const;
	void ResetCamera() const;
	UPROPERTY(EditAnywhere, Category="Camera Movement")
	float ScreenEdgeHigh = 0.98;
	UPROPERTY(EditAnywhere, Category="Camera Movement")
	float ScreenEdgeLow = 0.02;
	UPROPERTY(EditAnywhere, Category="Camera Movement")
	float ViewDistance = 1500.f;
	UPROPERTY(EditAnywhere, Category="Camera Movement")
	float PanSpeed = 1000.f;
	UPROPERTY(EditAnywhere, Category="Camera Movement")
	float CameraResetInterpSpeed = 10.f;
	bool bCameraReset = true;

	/** Game Ability System */
	UPROPERTY(EditAnywhere, Category="Game Ability System")
	TObjectPtr<ATopDownPlayerState> PlayerPS;
	UPROPERTY(EditAnywhere, Category="Game Ability System")
	TObjectPtr<APlayerCharacterController> PlayerPC;

	


	
};
