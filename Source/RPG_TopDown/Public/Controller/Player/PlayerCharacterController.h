// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"


/* Forward Declaration */
class UInputMappingContext;
class UInputAction;
class IHighlightActorInterface;
class ICameraMovementInterface;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:

	APlayerCharacterController();

	virtual void PlayerTick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	/** Mouse Cursor */
	void SetCursorSettings();
	
private:

	/** Input Variables */
	UPROPERTY(EditAnywhere, Category="Enhanced Input")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

	UPROPERTY(EditAnywhere, Category="Enhanced Input")
	TObjectPtr<UInputAction> InputActionMove;

	UPROPERTY(EditAnywhere, Category="Enhanced Input")
	TObjectPtr<UInputAction> InputActionCameraZoomInOut;

	UPROPERTY(EditAnywhere, Category="Enhanced Input")
	TObjectPtr<UInputAction> InputActionCameraPan;

	/** Input Functions */
	void Move(const FInputActionValue& InputActionValue);
	void CameraZoomInOut(const FInputActionValue& InputActionValue);

	/** Mouse Cursor */
	void CursorTrace();
	TScriptInterface<IHighlightActorInterface> LastActor;
	TScriptInterface<IHighlightActorInterface> ThisActor;

	/** Camera */
	TScriptInterface<ICameraMovementInterface> CameraMovementInterface;

};
