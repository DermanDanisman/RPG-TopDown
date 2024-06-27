// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interface/Interaction/HighlightActorInterface.h"
#include "Controller/Widget/OverlayWidgetController.h"
#include "EnemyCharacter.generated.h"

/* Forward Declaration */
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API AEnemyCharacter : public ABaseCharacter, public IHighlightActorInterface
{
	GENERATED_BODY()
	
public:
	
	AEnemyCharacter();

	/* Highlight Actor Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	/* Combat Interface */
	virtual int32 GetCharacterLevel() override;

	/* Delegate Signature from Overlay Widget */
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
protected:

	virtual void BeginPlay() override;

	/* Game Ability System */
	virtual void InitAbilityActorInfo() override;
	
	/*
	 * Now the reason I'm not going to make this replicated for the enemy is because we're only going to be concerned with checking the level on the server for AI controlled enemies.
	 * And that's because important things that will require that level in calculations will only be done on the server.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
};
