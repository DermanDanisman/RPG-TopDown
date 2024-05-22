// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interface/Interaction/HighlightActorInterface.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API AEnemyCharacter : public ABaseCharacter, public IHighlightActorInterface
{
	GENERATED_BODY()
	
public:
	
	AEnemyCharacter();

	/** Highlight Actor Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

protected:

	virtual void BeginPlay() override;

	/** Game Ability System */
	virtual void InitAbilityActorInfo() override;
};
