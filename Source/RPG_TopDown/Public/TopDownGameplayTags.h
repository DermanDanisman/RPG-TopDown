// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/*
 * What is the Singleton Pattern?
 * The singleton pattern is a design pattern that restricts the instantiation of a class to one "single" instance.
 * This is useful when exactly one object is needed to coordinate actions across the system.
 */

/**
 * TopDownGameplayTags
 * Singleton containing native GameplayTags
 * We're going to design this in a specific way with a singleton.
 * What you do is you create a static getter function, so you don't need an instance of the class to exist
 */

/**
 * Purpose:
 * The TopDownGameplayTags class is designed to centralize the management of gameplay tags within our project.
 * It ensures that all tags are consistent and easily accessible throughout our codebase.
 *
 * Implementation:
 * Singleton Pattern: This class uses the singleton pattern to ensure that only one instance of TopDownGameplayTags exists at any given time.
 * This is crucial for maintaining a single source of truth for your gameplay tags.
 *
 * Initialization: The class provides a method to initialize and request gameplay tags.
 * It ensures that all necessary tags are loaded and available for use when the game starts.
 *
 * Gameplay Tags: The class defines and requests various gameplay tags that are used throughout the game.
 * This approach avoids hardcoding tag strings in multiple places, reducing the risk of typos and making the code easier to maintain.
 */

struct FTopDownGameplayTags
{
public:
 // Static function to get the single instance of the class
 static const FTopDownGameplayTags& Get() { return GameplayTags; }
 // Static function to initialize native gameplay tags
 static void InitializeNativeGameplayTags();

 /*
  * Primary Attributes
  */

 FGameplayTag Attributes_Primary_Strength;
 FGameplayTag Attributes_Primary_Dexterity;
 FGameplayTag Attributes_Primary_Intelligence;
 FGameplayTag Attributes_Primary_Resilience;
 FGameplayTag Attributes_Primary_Vigor;
 
 /*
  * Secondary Attributes
  */
 FGameplayTag Attributes_Secondary_AttackPower;
 FGameplayTag Attributes_Secondary_SpellPower;
 FGameplayTag Attributes_Secondary_Armor;
 FGameplayTag Attributes_Secondary_MagicResistance;
 FGameplayTag Attributes_Secondary_ArmorPenetration;
 FGameplayTag Attributes_Secondary_CriticalHitChance;
 FGameplayTag Attributes_Secondary_CriticalHitDamage;
 FGameplayTag Attributes_Secondary_CriticalHitResistance;
 FGameplayTag Attributes_Secondary_Evasion;
 FGameplayTag Attributes_Secondary_MovementSpeed;
 FGameplayTag Attributes_Secondary_HealthRegeneration;
 FGameplayTag Attributes_Secondary_ManaRegeneration;
 FGameplayTag Attributes_Secondary_StaminaRegeneration;
 FGameplayTag Attributes_Secondary_MaximumHealth;
 FGameplayTag Attributes_Secondary_MaximumMana;
 FGameplayTag Attributes_Secondary_MaximumStamina;

 /*
  * Input Tags
  */
 FGameplayTag InputTag_LMB;
 FGameplayTag InputTag_RMB;
 FGameplayTag InputTag_1;
 FGameplayTag InputTag_2;
 FGameplayTag InputTag_3;
 FGameplayTag InputTag_4;

 /*
  *
  */
 FGameplayTag Damage;
 FGameplayTag Effects_HitReact;

private:
 // Static variable to hold the single instance of the class
 // Now, for a static variable like this, we need to go into the CPP file and explicitly declare the type there.
 static FTopDownGameplayTags GameplayTags;
};
