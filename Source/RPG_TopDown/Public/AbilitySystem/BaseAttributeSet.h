// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BaseAttributeSet.generated.h"

// Now we have this attribute accessors macro. And if we call ATTRIBUTE_ACCESSORS, then we don't have to use all four of these.
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FGameplayEffectContextInfo
{
	GENERATED_BODY()

	/*
	 * What Does Transient Mean?
	 * In Unreal Engine, marking a property with Transient means that this property should not be serialized.
	 * Serialization is the process of converting an object's state into a format that can be stored or transmitted and then reconstructed later.
	 * This is typically used for saving game state, replication over the network, etc.
	 */

	/*
	 * Why Use Transient?
	 * ---- Dynamically Set During Gameplay: ----
	 * The properties in FEffectProperties are meant to be set and used only during gameplay.
	 * They represent runtime information that is often calculated or retrieved dynamically.
	 * For instance, when an effect is applied, you fetch the relevant ability system component, actor, controller, and character based on the current state of the game.
	 * ---- Avoid Unnecessary Serialization: ----
	 * Since these properties are dynamically set and not part of the persistent state of the game, there's no need to serialize them.
	 * Serialization of unnecessary data can lead to performance overhead and potential issues during loading/saving of game states.
	 * ---- Network Efficiency: ----
	 * In multiplayer games, not serializing transient properties helps in reducing the amount of data that needs to be sent over the network.
	 * This ensures better performance and less bandwidth usage.
	 */
	
	UPROPERTY(Transient)
	UAbilitySystemComponent* AbilitySystemComponent = nullptr;
 
	UPROPERTY(Transient)
	AActor* AvatarActor = nullptr;
 
	UPROPERTY(Transient)
	AController* Controller = nullptr;
 
	UPROPERTY(Transient)
	ACharacter* Character = nullptr;
};

USTRUCT(BlueprintType)
struct FGameplayEffectContextDetails
{
	GENERATED_BODY()

	FGameplayEffectContextDetails()
	{
		SourceProperties = MakeShared<FGameplayEffectContextInfo>();
		TargetProperties = MakeShared<FGameplayEffectContextInfo>();
	}
    
	TSharedPtr<FGameplayEffectContextHandle> GameplayEffectContextHandle;
    
	TSharedPtr<FGameplayEffectContextInfo> SourceProperties;
 
	TSharedPtr<FGameplayEffectContextInfo> TargetProperties;
};

/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UBaseAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Most of our attributes are going to be replicated.
	 * There will be a couple of exceptions, and we'll get to those, but we want our variable to be replicated to all clients.
	 * So if it changes on the server, then clients will get the updated value.
	 * And if we change it via gameplay effects, we know those are predicted,
	 * which means that they'll be changed locally on a client and the server will be informed of that and so the server can change it.
	 * And once it changes on the server, all other clients need to know about that change.
	 */

	/**
	* What is RepNotify?
	* RepNotify, short for "Replicated Notification,"
	* is a feature in Unreal Engine that allows variables to be automatically replicated from the server to the clients.
	* When the value of a variable marked with ReplicatedUsing changes on the server, a specified function is called on the clients to handle this change.
	*/

	/**
	* How Does RepNotify Work?
	* Replication: Variables marked with ReplicatedUsing are automatically synchronized from the server to all clients.
	* Notification: When the server updates the variable, the specified function is called on each client.
	* Handling: This function, known as the "RepNotify" function, is used to handle any necessary updates or
	* changes that should occur when the variable's value is updated on the client.
	*/

	/** Gameplay Attribute Data */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Mana);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxMana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Stamina, Category="Vital Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Stamina);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxStamina, Category="Vital Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxStamina);

	/** Attribute Set Virtual Functions */
	/**
	 *	This is called just before any modification happens to an attribute's base value when an attribute aggregator exists.
	 *	This function should enforce clamping (presuming you wish to clamp the base value along with the final value in PreAttributeChange)
	 *	This function should NOT invoke gameplay related events or callbacks. Do those in PreAttributeChange() which will be called prior to the
	 *	final value of the attribute actually changing.
	 */
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	/**
	 *	Called just after a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made.
	 *	Note this is only called during an 'execute'. E.g., a modification to the 'base value' of an attribute.
	 *	It is not called during an application of a GameplayEffect, such as a 5 second +10 movement speed buff.
	 */
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;


	/** OnRep Attribute Functions */
	/*
	 * These functions are declared to handle the changes to Health and Mana.
	 * They now receive the old value of the attribute as a parameter.
	 */
	
	// Called when the variables are updated on the client side.
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	// Called when the variables are updated on the client side.
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	
	// Called when the variables are updated on the client side.
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	// Called when the variables are updated on the client side.
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	
	// Called when the variables are updated on the client side.
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;
	// Called when the variables are updated on the client side.
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const;

private:

	/**
	 * The function is responsible for setting up and populating the FGameplayEffectExecutionContext struct.
	 */
	void InitializeEffectExecutionContext(const FGameplayEffectModCallbackData& Data, FGameplayEffectContextDetails& GameplayEffectContextDetails) const;
};
