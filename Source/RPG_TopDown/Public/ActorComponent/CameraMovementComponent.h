// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/Camera/CameraMovementInterface.h"
#include "CameraMovementComponent.generated.h"

/* Forward Declaration */
class USpringArmComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPG_TOPDOWN_API UCameraMovementComponent : public UActorComponent, public ICameraMovementInterface
{
	GENERATED_BODY()

public:	
	UCameraMovementComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Setter Functions */
	void SetCameraSpringArm(USpringArmComponent* SpringArm) { CameraSpringArm = SpringArm; }

	/** Getter Functions */
	float GetTargetArmLength() const { return TargetArmLength; }

	/** Camera Movement Interface */
	virtual void CameraZoom(float ActionInput) override;

protected:

	virtual void BeginPlay() override;

private:
	
	// Updates the camera offset based on mouse position
	void UpdateCameraOffset(float DeltaSeconds, const FVector2D& MousePositionPercent) const;

	// Resets the camera to the default position
    void ResetCamera() const;

    // Retrieves the mouse position as a percentage of the viewport size
    FVector2D GetMousePositionPercent() const;

	// Pointer to the Spring Arm component controlling the camera
	TObjectPtr<USpringArmComponent> CameraSpringArm;

	// Configurable properties for camera movement
	UPROPERTY(EditAnywhere, Category="Camera Movement")
	float ScreenEdgeHigh = 0.98;
	UPROPERTY(EditAnywhere, Category="Camera Movement")
	float ScreenEdgeLow = 0.02;
	UPROPERTY(EditAnywhere, Category="Camera Movement")
	float ViewDistance = 1500.f;
	UPROPERTY(EditAnywhere, Category="Camera Movement")
	float PanSpeed = 1000.f;
	UPROPERTY(EditAnywhere, Category="Camera Movement")
	float CameraResetInterpSpeed = 3.f;

	// Configurable properties for camera zoom
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = true), Category="Camera Properties")
	float TargetArmLength = 1500.f;
	UPROPERTY(EditAnywhere, Category="Camera Properties")
	float TargetArmLengthMin = 250.f;
	UPROPERTY(EditAnywhere, Category="Camera Properties")
	float CameraZoomSpeed = 1000.f;
};
