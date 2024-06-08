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

	/*
	 * Vital Attributes
	 */
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Stamina, Category="Vital Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Stamina);

	/*
	 * Primary Attributes
	 */

	// Strength: Increases physical attack power.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Strength, Category="Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Strength);

	// Dexterity: Increases critical hit chance, critical hit damage, evasion, and movement speed.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Dexterity, Category="Primary Attributes")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Dexterity);

	// Intelligence: Increases spell power, magic resistance, mana regeneration, and max mana.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Intelligence, Category="Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Intelligence);

	// Resilience: Increases armor, magic resistance, armor penetration, and evasion.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Resilience, Category="Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Resilience);

	// Vigor: Increases health regeneration, stamina regeneration, max health, and max stamina.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Vigor, Category="Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Vigor);


	/*
	 * Secondary Attributes
	 */

	// Dependent on Strength and Weapon Damage -- Attack Power = Strength * 2 + Weapon Damage
	// Description: Increases the damage dealt with physical attacks.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_AttackPower, Category="Secondary Attributes")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, AttackPower);

	// Dependent on Intelligence -- Spell Power = (1.5 × (Intelligence + 10)) + 0
	// Description: Increases the effectiveness of magical spells, boosting their damage or healing output.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_SpellPower, Category="Secondary Attributes")
	FGameplayAttributeData SpellPower;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, SpellPower);

	// Dependent on Resilience -- Armor = (1.2 × (Resilience + 5)) + 0
	// Description: Provides physical damage mitigation, reducing the damage taken from physical attacks.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Armor, Category="Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Armor);

	// Dependent on Resilience -- Magic Resistance = (0.8 × (Resilience + 5)) + 0
	// Description: Reduces the damage taken from magical attacks, making the character more resilient against spells.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MagicResistance, Category="Secondary Attributes")
	FGameplayAttributeData MagicResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MagicResistance);

	// Dependent on Resilience -- Armor Penetration = (0.5 × (Resilience + 3)) + 0
	// Description: Increases the ability to bypass enemy armor, resulting in higher damage dealt to armored foes.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ArmorPenetration, Category="Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ArmorPenetration);

	// Dependent on Dexterity and Armor Penetration -- Critical Hit Chance = (0.4 × (Dexterity + 2)) + 0
	// Description: Increases the likelihood of landing a critical hit, which deals additional damage.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitChance, Category="Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CriticalHitChance);

	// Dependent on Dexterity (Physical Damage) or Intelligence (Magic Damage) -- Critical Hit Damage = (1.2 × (Intelligence + 0)) + 5
	// Description: Increases the damage dealt by critical hits, making them more powerful.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitDamage, Category="Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CriticalHitDamage);

	// Dependent on Armor -- Critical Hit Resistance = (0.5 × (Armor + 1)) + 0
	// Description: Reduces the chance of receiving a critical hit from enemies, lowering the probability of critical damage.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitResistance, Category="Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CriticalHitResistance);

	// Dependent on Dexterity and Resilience -- Evasion = (0.3 × (Dexterity + Resilience)) + 2
	// Description: Increases the chance to dodge attacks, avoiding damage completely.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Evasion, Category="Secondary Attributes")
	FGameplayAttributeData Evasion;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Evasion);

	// Dependent on Dexterity -- Movement Speed = (0.4 × (Dexterity + 5)) + 100
	// Description: Increases the character's speed of movement, aiding in both combat and exploration.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MovementSpeed, Category="Secondary Attributes")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MovementSpeed);

	// Dependent on Vigor -- Health Regeneration = (0.5 × (Vigor + 1)) + 0
	// Description: Increases the rate at which health regenerates over time, aiding in recovery outside of combat.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_HealthRegeneration, Category="Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, HealthRegeneration);

	// Dependent on Intelligence -- Mana Regeneration = (1.0 × (Intelligence + 0)) + 3
	// Description: Increases the rate at which mana regenerates over time, ensuring a steady supply of mana for spells.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ManaRegeneration, Category="Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ManaRegeneration);

	// Dependent on Vigor -- Stamina Regeneration = (0.5 × (Vigor + 1)) + 0
	// Description: Increases the rate at which stamina regenerates over time, aiding in recovery outside of combat.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_StaminaRegeneration, Category="Secondary Attributes")
	FGameplayAttributeData StaminaRegeneration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, StaminaRegeneration);

	// Dependent on Vigor -- Max Health = (10 × (Vigor + 0)) + 50
	// Description: Increases the total amount of health, allowing the character to endure more damage before falling in battle.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth);

	// Dependent on Intelligence -- Max Mana = (5 × (Intelligence + 0)) + 25
	// Description: Increases the total amount of mana, providing a larger pool for casting spells.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxMana);

	// Dependent on Vigor -- Max Stamina = (10 × (Vigor + 0)) + 50
	// Description: Increases the total amount of stamina, providing a larger pool for physical activities and abilities.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxStamina, Category="Secondary Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxStamina);

	/*
	 * Attribute Virtual Functions
	 */
	/**
	 *	This is called just before any modification happens to an attribute's base value when an attribute aggregator exists.
	 *	This function should enforce clamping (presuming you wish to clamp the base value along with the final value in PreAttributeChange)
	 *	This function should NOT invoke gameplay related events or callbacks. Do those in PreAttributeChange() which will be called prior to the
	 *	final value of the attribute actually changing.
	 *	PreAttributeBaseChange is called on instant effects or effects with a period since they change the base value (potions, fire, etc.).
	 *	It is not called on effects with duration/infinite with no period like a buff/debuff
	 */
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	/**
	 * PreAttributeChange is called in all cases since the final attribute value is changed in each case,
	 * However the float& NewValue that is passed in parameter is only the final attribute value, not the base value,
	 * Hence you cannot clamp modifications made to the base value by clamping NewValue here. You can react to it
	 */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/**
	 *	Called just after a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made.
	 *	Note this is only called during an 'execute'. E.g., a modification to the 'base value' of an attribute.
	 *	It is not called during an application of a GameplayEffect, such as a 5 second +10 movement speed buff.
	 *	PostGameplayEffectExecute is called last and in the same situations as PreAttributeBaseChange.
	 *	It is not called on effects with duration/infinite with no period like a buff/debuff.
	 */
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;


	/*
	 * OnRep Attribute Functions
	 */
	
	/*
	 * These functions are declared to handle the changes to attributes on the client side.
	 * They now receive the old value of the attribute as a parameter.
	 * Called when the variables are updated on the client side.
	 */

	/*
	 * Vital Attributes
	 */
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;

	/*
	 * Primary Attributes
	 */
	
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	
	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const;
	
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
	
	UFUNCTION()
	void OnRep_Resilience (const FGameplayAttributeData& OldResilience ) const;
	
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	/*
	 * Secondary Attributes
	 */
	
	UFUNCTION()
	void OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower) const;
	
	UFUNCTION()
	void OnRep_SpellPower(const FGameplayAttributeData& OldSpellPower) const;
	
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;
	
	UFUNCTION()
	void OnRep_MagicResistance(const FGameplayAttributeData& OldMagicResistance) const;
	
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;
	
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;
	
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;
	
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;
	
	UFUNCTION()
	void OnRep_Evasion(const FGameplayAttributeData& OldEvasion) const;

	UFUNCTION()
	void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

	UFUNCTION()
	void OnRep_StaminaRegeneration(const FGameplayAttributeData& OldStaminaRegeneration) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const;

private:

	/**
	 * The function is responsible for setting up and populating the FGameplayEffectExecutionContext struct.
	 */
	void InitializeEffectExecutionContext(const FGameplayEffectModCallbackData& Data, FGameplayEffectContextDetails& GameplayEffectContextDetails) const;
};
