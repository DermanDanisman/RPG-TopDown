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


	/** OnRep Attribute Functions */
	/**
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
};
