// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEffectActor.generated.h"

/*
 * Gameplay Ability System (GAS) Concepts
 * Gameplay Effects: These are objects that define modifications to attributes (like health, mana, etc.), application of buffs/debuffs, and other gameplay-related changes.
 * Ability System Component (ASC): This component is responsible for managing abilities and gameplay effects on an actor.
 * Gameplay Effect Context: Provides context about the effect, such as who applied it, where it was applied, etc.
 * Gameplay Effect Specification (Spec): Describes the specifics of the effect, such as its level, magnitude, duration, and context.
 */

/*
 * Practical Example
 * Imagine you have a healing potion that a player can use to heal themselves.
 * The healing effect is a gameplay effect defined in a blueprint.
 * When the player uses the potion, you want to apply this healing effect to the player character.
 * TargetAbilitySystemComponent: The UAbilitySystemComponent of the player character.
 * GameplayEffectClass: The class of the healing effect.
 * EffectContextHandle: Contains information like the source of the healing effect (the potion).
 * EffectSpecHandle: Specifies the healing effect with its level and context.
 * ApplyGameplayEffectSpecToSelf: Applies the healing effect to the player character.
 */

/** Forward Declaring Classes */
class USphereComponent;
class UGameplayEffect;

UCLASS()
class RPG_TOPDOWN_API ABaseEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseEffectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Gameplay Effect */
	UPROPERTY(EditAnywhere, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass);
private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

};
