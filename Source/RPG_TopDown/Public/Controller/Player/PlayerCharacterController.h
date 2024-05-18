// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

/** Forward Declaring Classes */
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:

	APlayerCharacterController();

protected:
	
	virtual void BeginPlay() override;

	void SetCursorSettings();

	void Move(const FInputActionValue& InputActionValue);
	
private:

	/** Input */
	UPROPERTY(EditAnywhere, Category="Enhanced Input")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

	UPROPERTY(EditAnywhere, Category="Enhanced Input")
	TObjectPtr<UInputAction> IA_Move;
};
