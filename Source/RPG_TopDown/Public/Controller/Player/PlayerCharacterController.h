// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "PlayerCharacterController.generated.h"


/* Forward Declaration */
class UInputMappingContext;
class UInputAction;
class IHighlightActorInterface;
class ICameraMovementInterface;
class UTopDownInputConfigDataAsset;
class UBaseAbilitySystemComponent;
class USplineComponent;
class UDamageTextWidgetComponent;
struct FInputActionValue;

/**
 * APlayerCharacterController
 * This class manages player input and activates abilities based on input actions and gameplay tags.
 */
UCLASS()
class RPG_TOPDOWN_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:

	APlayerCharacterController();

	virtual void PlayerTick(float DeltaTime) override;

	// Client RPC
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* Target);

protected:
	
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:

	/* Input Functions */
	void Move(const FInputActionValue& InputActionValue);
	void CameraZoomInOut(const FInputActionValue& InputActionValue);

	/* Custom Input Functions */
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	void ShiftPressed() { bShiftKeyDown = true; }
	void ShiftReleased() { bShiftKeyDown = false; }

	/* Input Variables */
	UPROPERTY(EditAnywhere, Category="Input|Enhanced")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;
	UPROPERTY(EditAnywhere, Category="Input|Custom")
	TObjectPtr<UTopDownInputConfigDataAsset> InputConfigDataAsset;
	UPROPERTY(EditAnywhere, Category="Input|Enhanced")
	TObjectPtr<UInputAction> InputActionMove;
	UPROPERTY(EditAnywhere, Category="Input|Enhanced")
	TObjectPtr<UInputAction> InputActionShift;
	bool bShiftKeyDown = false;
	UPROPERTY(EditAnywhere, Category="Input|Enhanced")
	TObjectPtr<UInputAction> InputActionCameraZoomInOut;
	UPROPERTY(EditAnywhere, Category="Input|Enhanced")
	TObjectPtr<UInputAction> InputActionCameraPan;
	
	/* Mouse Cursor */
	void CursorTrace();
	void SetCursorSettings();
	TScriptInterface<IHighlightActorInterface> LastActor;
	TScriptInterface<IHighlightActorInterface> ThisActor;
	
	/* Camera */
	TScriptInterface<ICameraMovementInterface> CameraMovementInterface;

	/* References */
	UBaseAbilitySystemComponent* GetAbilitySystemComponent();
	UPROPERTY()
	TObjectPtr<UBaseAbilitySystemComponent> BaseAbilitySystemComponent;
	UPROPERTY(EditDefaultsOnly, Category="References|Classes")
	TSubclassOf<UDamageTextWidgetComponent> DamageTextWidgetComponentClass;

	/* Character Movement */
	void AutoRun();
	FVector CachedMoveDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;
	UPROPERTY(EditDefaultsOnly,	Category="Character Movement|Click And Move")
	float AutoRunAcceptanceRadius = 25.f;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
	
};
