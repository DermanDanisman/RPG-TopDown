// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
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
class UAbilitySystemComponent;

// ENUM class for effect application and removal policies
UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyEffectOnOverlap,
	ApplyEffectOnEndOverlap,
	DoNotApplyEffect
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveEffectOnEndOverlap,
	DoNotRemoveEffect
};

// Struct to store properties of gameplay effects
USTRUCT(BlueprintType)
struct FAppliedGameplayEffectProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Gameplay Effects Properties")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Gameplay Effects Properties")
	EEffectApplicationPolicy GameplayEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApplyEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Gameplay Effects Properties")
	EEffectRemovalPolicy GameplayEffectRemovalPolicy = EEffectRemovalPolicy::DoNotRemoveEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Gameplay Effects Properties")
	int StackRemovalCount = -1;
};

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
	
	/** Gameplay Effect Properties Struct */
	// Arrays to store different types of gameplay effects
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TArray<FAppliedGameplayEffectProperties> InstantGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TArray<FAppliedGameplayEffectProperties> DurationGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TArray<FAppliedGameplayEffectProperties> PeriodicGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TArray<FAppliedGameplayEffectProperties> InfiniteGameplayEffects;

	UPROPERTY(EditAnywhere, Category="Applied Effects")
	bool bDestroyActorOnEffectApplication = false;
	
	UPROPERTY(EditAnywhere, Category="Applied Effects")
	bool bDestroyActorOnEffectRemoval = false;

	/** Gameplay Effect Functions */
	UFUNCTION(BlueprintCallable, Category="Gameplay Effect Functions")
	void OnOverlap(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable, Category="Gameplay Effect Functions")
	void OnEndOverlap(AActor* TargetActor);

	// Deprecated for now
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveGameplayEffectHandlesMap;


	
private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	void ApplyGameplayEffectToTarget(AActor* Target, const FAppliedGameplayEffectProperties& AppliedGameplayEffectProperties);

	void RemoveGameplayEffectFromTarget(AActor* TargetActor, const FAppliedGameplayEffectProperties& AppliedGameplayEffectProperties);

	void ApplyAllGameplayEffects(AActor* TargetActor, const EEffectApplicationPolicy& EffectApplicationPolicy);

	void RemoveAllGameplayEffects(AActor* TargetActor, const EEffectRemovalPolicy& EffectRemovalPolicy);
};
