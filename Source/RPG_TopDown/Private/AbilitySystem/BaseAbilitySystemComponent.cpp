// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAbilitySystemComponent.h"

#include "TopDownGameplayTags.h"

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
