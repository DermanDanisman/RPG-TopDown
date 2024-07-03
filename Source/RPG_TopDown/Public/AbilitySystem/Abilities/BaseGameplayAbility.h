// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

/**
 * UBaseGameplayAbility
 * This class extends UGameplayAbility to include additional functionality specific to your game.
 */
UCLASS()
class RPG_TOPDOWN_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()



public:

	/**
	 * StartupInputTag: This is used to assign an input tag to the ability at startup.
	 * This tag helps in binding the ability to an input action via the input system.
	 * Now a gameplay ability might have its input tag changed at runtime, so this variable is really only for the startup input tag.
	 * So it only applies if this is one of our startup abilities given at the beginning of the game. This is not something that we should change at runtime.
	 */
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	FScalableFloat Damage;
	
};
