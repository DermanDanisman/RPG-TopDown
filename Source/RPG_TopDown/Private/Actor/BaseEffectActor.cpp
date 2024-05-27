// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BaseEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Sets default values
ABaseEffectActor::ABaseEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultSceneRoot);
	
}

// Called when the game starts or when spawned
void ABaseEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	/*
	* Purpose: Retrieves the UAbilitySystemComponent from the target actor.
	* Details: This function is versatile as it checks if the actor implements the IAbilitySystemInterface and
	* if not, it tries to find an AbilitySystemComponent directly on the actor.
	* The UAbilitySystemComponent is crucial as it manages abilities, gameplay effects, and attributes.
	*/
	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetAbilitySystemComponent == nullptr) return;

	// GameplayEffectClass can not be unset, so we need to check.
	checkf(GameplayEffectClass, TEXT("Gameplay Effect Class is UNSET! in Base Effect Actor blueprint."));
	
	/*
	* Purpose: Creates a context handle for the gameplay effect.
	* Details: The FGameplayEffectContextHandle encapsulates contextual information about the effect,
	* such as the source of the effect, targets, and other relevant data.
	* This context is essential for applying and replicating gameplay effects properly.
	* Handle that wraps a FGameplayEffectContext or subclass, to allow it to be polymorphic and replicate properly
	* The handle is a lightweight wrapper that stores the actual effect context as a pointer. It's called data.
	*/
	FGameplayEffectContextHandle EffectContextHandle =TargetAbilitySystemComponent->MakeEffectContext();
	
	// Sets the object this effect was created from.
	/*
	* Purpose: Adds the ABaseEffectActor as the source object of the effect.
	* Details: This helps in identifying where the effect originated from, which can be useful for logging, debugging,
	* or applying additional logic based on the source.
	*/
	EffectContextHandle.AddSourceObject(this);
	/*
	* Purpose: Creates a specification handle for the gameplay effect.
	* Details: The FGameplayEffectSpecHandle contains all the necessary details to apply a gameplay effect,
	* such as its magnitude, duration, and the previously created effect context.
	* The 1.f represents the level of the effect, which can influence its strength.
	*/
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);

	/*
	 * Details: This line effectively executes the gameplay effect on the actor that owns the UAbilitySystemComponent.
	 * The EffectSpecHandle.Data.Get() part retrieves the raw pointer from the shared pointer Data,
	 * and * dereferences it to pass a reference to the function.
	 */
	TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

