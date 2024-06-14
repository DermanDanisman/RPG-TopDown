// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CameraMovementInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCameraMovementInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_TOPDOWN_API ICameraMovementInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Pure virtual C++ function when put "= 0" after "()"
	// Not need to provide definition for this function
	// This class is now considered as Abstract class, must be derived from in other classes
	virtual void CameraZoom(float ActionInput) = 0;
	virtual void CameraEdgeScrolling(float DeltaSeconds, const FVector2D& MousePositionPercent) = 0;
};
