// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HighlightActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHighlightActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_TOPDOWN_API IHighlightActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Pure virtual C++ function when put "= 0" after "()"
	// Not need to provide definition for this function
	// This class is now considered as Abstract class, must be derived from in other classes
	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0;
};
