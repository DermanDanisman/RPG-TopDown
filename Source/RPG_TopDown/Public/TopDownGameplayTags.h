// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * TopDownGameplayTags
 * Singleton containing native GameplayTags
 * We're going to design this in a specific way with a singleton.
 * What you do is you create a static getter function, so you don't need an instance of the class to exist
 */
struct FTopDownGameplayTags
{
public:
 static const FTopDownGameplayTags& Get() { return GameplayTags; }
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
 
protected:

private:
 // Now, for a static variable like this, we need to go into the CPP file and explicitly declare the type there.
 static FTopDownGameplayTags GameplayTags;
};
