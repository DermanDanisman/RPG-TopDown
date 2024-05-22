// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
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
class RPG_TOPDOWN_API APlayerCharacter : public ABaseCharacter, public IHighlightActorInterface
{
	GENERATED_BODY()

public:

	APlayerCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Highlight Actor Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

protected:

	/** Game Ability System */
	virtual void InitAbilityActorInfo() override;
	
private:

	/** Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = true), Category="Camera")
	TObjectPtr<USpringArmComponent> CameraSpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = true), Category="Camera")
	TObjectPtr<UCameraComponent> Camera;

	/** Game Ability System */
	UPROPERTY(EditAnywhere, Category="Game Ability System")
	TObjectPtr<ATopDownPlayerState> PlayerPS;
	UPROPERTY(EditAnywhere, Category="Game Ability System")
	TObjectPtr<APlayerCharacterController> PlayerPC;
};
