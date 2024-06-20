// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAbilitySystemComponent.h"

#include "AssetTypeCategories.h"
#include "TopDownGameplayTags.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"

// Binds the delegate to handle effects applied to the ability system component.
void UBaseAbilitySystemComponent::BindOnGameplayEffectAppliedDelegateToSelf()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UBaseAbilitySystemComponent::EffectAppliedToSelf);

	//const FTopDownGameplayTags GameplayTags = FTopDownGameplayTags::Get();
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Tag: %s"), *GameplayTags.Attributes_Primary_Strength.ToString()));
}

void UBaseAbilitySystemComponent::EffectAppliedToSelf(UAbilitySystemComponent* AbilitySystemComponent,
                                                      const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer GameplayTagContainer;
	/*
	 * GetAllAssetTags()
	 * Purpose:
	 * GetAllAssetTags() retrieves all the gameplay tags that are explicitly defined in the GameplayEffect asset.
	 * These tags are associated with the effect itself and typically represent static, predefined attributes or conditions.
	 * Usage:
	 * Asset tags are generally used to categorize or identify effects.
	 * They can be useful for filtering, applying specific logic, or triggering responses based on the presence of these tags.
	 * Example:
	 * A healing effect might have an asset tag like Effect.Healing.
	 * A damage-over-time effect might have an asset tag like Effect.DamageOverTime.
	 */
	GameplayEffectSpec.GetAllAssetTags(GameplayTagContainer);
	// Any class who binds to GameplayEffectAssetTags delegate will receive a GameplayTagContainer filled with the asset tags.
	GameplayEffectAssetTags.Broadcast(GameplayTagContainer);

	
	//FGameplayTagContainer GameplayGrantedTagContainer;
	/*
	 * GetAllGrantedTags()
	 * Purpose:
	 * GetAllGrantedTags() retrieves all the gameplay tags that are dynamically granted to the target when the effect is applied.
	 * These tags can be conditionally granted based on the effect's configuration or the state of the target.
	 * Usage:
	 * Granted tags are used to modify the state of the target actor temporarily.
	 * These tags can affect how other abilities or effects interact with the target while the effect is active.
	 * Example:
	 * Applying a stealth effect might grant the tag State.Stealth, which can prevent enemies from detecting the player.
	 * A buff effect might grant the tag State.Buffed, enabling the player to benefit from certain abilities or resistances.
	 */
	//GameplayEffectSpec.GetAllGrantedTags(GameplayGrantedTagContainer);
}

void UBaseAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		/*
		 * Now we want our gameplay abilities to have the concept of an input tag, but we'd like to be able to change this at runtime.
		 * For example, we might have an ability mapped to the left mouse button, and we may want to reassign it to the right mouse button.
		 * So having a variable on the UBaseGameplayAbility class is great for something like the startup input tag,
		 * but if we want to be able to change it, we can't really use variables on the UBaseGameplayAbility class.
		 * Well, the GameplayAbilitySpec has a gameplay tag container specifically for tags that can be added or removed dynamically throughout the game.
		 * So that's perfect for our input tag. And as our ability system component adds the abilities for the first time at the beginning of the game.
		 * Since these are startup abilities, we can check that startup input tag, and we can add those to our ability spec for that given ability.
		 */
		
		if (const UBaseGameplayAbility* BaseGameplayAbility = Cast<UBaseGameplayAbility>(GameplayAbilitySpec.Ability))
		{
			GameplayAbilitySpec.DynamicAbilityTags.AddTag(BaseGameplayAbility->StartupInputTag);
			GiveAbility(GameplayAbilitySpec);
		}
	}
}

// Ability activation function when Input is held by the player for the given ability
void UBaseAbilitySystemComponent::ActivateAbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& ActivatableAbilitySpecs : GetActivatableAbilities())
	{
		if (ActivatableAbilitySpecs.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(ActivatableAbilitySpecs);
			if (!ActivatableAbilitySpecs.IsActive())
			{
				// We have to call tri activate ability because there may be things that prevent the ability from being activated. So we have to try to activate it.
				TryActivateAbility(ActivatableAbilitySpecs.Handle);
			}
		}
	}
}

// Ability activation function when Input is released by the player for the given ability
void UBaseAbilitySystemComponent::ActivateAbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& ActivatableAbilitySpecs : GetActivatableAbilities())
	{
		if (ActivatableAbilitySpecs.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(ActivatableAbilitySpecs);
		}
	}
}
