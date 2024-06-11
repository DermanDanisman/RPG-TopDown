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

void ABaseEffectActor::OnOverlap(AActor* TargetActor)
{
	ApplyAllGameplayEffects(TargetActor, EEffectApplicationPolicy::ApplyEffectOnOverlap);
}

void ABaseEffectActor::OnEndOverlap(AActor* TargetActor)
{
	ApplyAllGameplayEffects(TargetActor, EEffectApplicationPolicy::ApplyEffectOnEndOverlap);
	RemoveAllGameplayEffects(TargetActor, EEffectRemovalPolicy::RemoveEffectOnEndOverlap);
}

void ABaseEffectActor::ApplyGameplayEffectToTarget(AActor* Target, const FAppliedGameplayEffectProperties& AppliedGameplayEffectProperties)
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
	checkf(AppliedGameplayEffectProperties.GameplayEffectClass, TEXT("Gameplay Effect Class is UNSET! in Base Effect Actor blueprint."));
	
	/*
	 * Purpose: Creates a context handle for the gameplay effect.
	 * Details: The FGameplayEffectContextHandle encapsulates contextual information about the effect,
	 * such as the source of the effect, targets, and other relevant data.
	 * This context is essential for applying and replicating gameplay effects properly.
	 * Handle that wraps a FGameplayEffectContext or subclass, to allow it to be polymorphic and replicate properly
	 * The handle is a lightweight wrapper that stores the actual effect context as a pointer. It's called data.
	 */
	FGameplayEffectContextHandle EffectContextHandle =TargetAbilitySystemComponent->MakeEffectContext();
	

	/*
	 * Purpose: Adds the ABaseEffectActor as the source object of the effect.
	 * Details: This helps in identifying where the effect originated from, which can be useful for logging, debugging,
	 * or applying additional logic based on the source.
	 */
	// Sets the object this effect was created from.
	EffectContextHandle.AddSourceObject(this);
	EffectContextHandle.Get()->SetEffectCauser(this);
	
	/*
	 * Purpose: Creates a specification handle for the gameplay effect.
	 * Details: The FGameplayEffectSpecHandle contains all the necessary details to apply a gameplay effect,
	 * such as its magnitude, duration, and the previously created effect context.
	 * The 1.f represents the level of the effect, which can influence its strength.
	 */
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(AppliedGameplayEffectProperties.GameplayEffectClass, ActorLevel, EffectContextHandle);

	/*
	 * Details: This line effectively executes the gameplay effect on the actor that owns the UAbilitySystemComponent.
	 * The EffectSpecHandle.Data.Get() part retrieves the raw pointer from the shared pointer Data,
	 * and * dereferences it to pass a reference to the function.
	 * This method is used to access the underlying object directly.
	 * 
	 * ApplyGameplayEffectSpecToSelf has a return type. It's not void, it's active gameplay effect handle.
	 * So once you've applied a gameplay effect, that gameplay effect becomes active and these apply functions return a FActiveGameplayEffectHandle to that effect.
	 * So we can always use that handle for something later, such as removing it if it's an infinite gameplay effect.
	 */
	TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	// If true, Destroys actor on effect application.
	if (bDestroyActorOnEffectApplication) Destroy();

	/*
	 * DEBUG: Count the active gameplay effects
	 */
	const int EffectCount = TargetAbilitySystemComponent->GetGameplayEffectCount(AppliedGameplayEffectProperties.GameplayEffectClass, TargetAbilitySystemComponent);
	const FString ClassName = AppliedGameplayEffectProperties.GameplayEffectClass->GetName();
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Active Effect Name and Count: %s and %d"), *ClassName, EffectCount));
}

void ABaseEffectActor::RemoveGameplayEffectFromTarget(AActor* TargetActor, const FAppliedGameplayEffectProperties& AppliedGameplayEffectProperties)
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!IsValid(TargetAbilitySystemComponent)) return;

	TargetAbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(AppliedGameplayEffectProperties.GameplayEffectClass,
		TargetAbilitySystemComponent, AppliedGameplayEffectProperties.StackRemovalCount);
	
	// If true, Destroys actor on effect removal.
	if (bDestroyActorOnEffectRemoval) Destroy();

	/*
	 * DEBUG: Count the active gameplay effects
	 */
	const int EffectCount = TargetAbilitySystemComponent->GetGameplayEffectCount(AppliedGameplayEffectProperties.GameplayEffectClass, TargetAbilitySystemComponent);
	const FString ClassName = AppliedGameplayEffectProperties.GameplayEffectClass->GetName();
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Active Effect Name and Count: %s and %d"), *ClassName, EffectCount));
}

void ABaseEffectActor::ApplyAllGameplayEffects(AActor* TargetActor, const EEffectApplicationPolicy& EffectApplicationPolicy)
{
	// Apply instant gameplay effects
	for (const FAppliedGameplayEffectProperties& InstantGameplayEffect : InstantGameplayEffects)
	{
		if (InstantGameplayEffect.GameplayEffectApplicationPolicy == EffectApplicationPolicy)
		{
			ApplyGameplayEffectToTarget(TargetActor, InstantGameplayEffect);
		}
	}

	// Apply duration gameplay effects
	for (const FAppliedGameplayEffectProperties& DurationGameplayEffect : DurationGameplayEffects)
	{
		if (DurationGameplayEffect.GameplayEffectApplicationPolicy == EffectApplicationPolicy)
		{
			ApplyGameplayEffectToTarget(TargetActor, DurationGameplayEffect);
		}
	}

	// Apply periodic gameplay effects
	for (const FAppliedGameplayEffectProperties& PeriodicGameplayEffect : PeriodicGameplayEffects)
	{
		if (PeriodicGameplayEffect.GameplayEffectApplicationPolicy == EffectApplicationPolicy)
		{
			ApplyGameplayEffectToTarget(TargetActor, PeriodicGameplayEffect);
		}
	}
	
	// Apply infinite gameplay effects
	for (const FAppliedGameplayEffectProperties& InfiniteGameplayEffect : InfiniteGameplayEffects)
	{
		if (InfiniteGameplayEffect.GameplayEffectApplicationPolicy == EffectApplicationPolicy)
		{
			ApplyGameplayEffectToTarget(TargetActor, InfiniteGameplayEffect);
		}
	}
}

void ABaseEffectActor::RemoveAllGameplayEffects(AActor* TargetActor, const EEffectRemovalPolicy& EffectRemovalPolicy)
{
	// Remove duration gameplay effects
	for (const FAppliedGameplayEffectProperties& DurationGameplayEffect : DurationGameplayEffects)
	{
		if (DurationGameplayEffect.GameplayEffectRemovalPolicy == EffectRemovalPolicy)
		{
			RemoveGameplayEffectFromTarget(TargetActor, DurationGameplayEffect);
		}
	}

	// Remove periodic gameplay effects
	for (const FAppliedGameplayEffectProperties& PeriodicGameplayEffect : PeriodicGameplayEffects)
	{
		if (PeriodicGameplayEffect.GameplayEffectRemovalPolicy == EffectRemovalPolicy)
		{
			RemoveGameplayEffectFromTarget(TargetActor, PeriodicGameplayEffect);
		}
	}
	
	// Remove infinite gameplay effects
	for (const FAppliedGameplayEffectProperties& InfiniteGameplayEffect : InfiniteGameplayEffects)
	{
		if (InfiniteGameplayEffect.GameplayEffectRemovalPolicy == EffectRemovalPolicy)
		{
			RemoveGameplayEffectFromTarget(TargetActor, InfiniteGameplayEffect);
		}
	}
}

