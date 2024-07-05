// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownGameplayTags.h"
#include "GameplayTagsManager.h"

// Definition of the static instance
FTopDownGameplayTags FTopDownGameplayTags::GameplayTags;

// Static function to initialize native gameplay tag
void FTopDownGameplayTags::InitializeNativeGameplayTags()
{
	// Initialize native gameplay tags here

	/*
	 * Primary Attributes
	 */
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"),
			FString("Increases physical attack power. "));
	
	GameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Dexterity"),
			FString("Increases critical hit chance, critical hit damage, evasion, and movement speed. "));
	
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"),
			FString("Increases spell power, magic resistance, mana regeneration, and max mana. "));
	
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"),
			FString("Increases armor, magic resistance, armor penetration, and evasion. "));
	
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"),
			FString("Increases health regeneration, stamina regeneration, max health, and max stamina. "));
	
	/*
	 * Secondary Attributes
	 */
	GameplayTags.Attributes_Secondary_AttackPower = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.AttackPower"),
			FString("Increases the damage dealt with physical attacks. "));
	
	GameplayTags.Attributes_Secondary_SpellPower = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.SpellPower"),
			FString("Increases the effectiveness of magical spells, boosting their damage or healing output. "));
	
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"),
			FString("Provides physical damage mitigation, reducing the damage taken from physical attacks. "));
	
	GameplayTags.Attributes_Secondary_MagicResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MagicResistance"),
			FString("Reduces the damage taken from magical attacks, making the character more resilient against spells. "));
	
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"),
			FString("Increases the ability to bypass enemy armor, resulting in higher damage dealt to armored foes. "));
	
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"),
			FString("Increases the likelihood of landing a critical hit, which deals additional damage. "));
	
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"),
			FString("Increases the damage dealt by critical hits, making them more powerful. "));
	
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"),
			FString("Reduces the chance of receiving a critical hit from enemies, lowering the probability of critical damage. "));
	
	GameplayTags.Attributes_Secondary_Evasion = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Evasion"),
			FString("Increases the chance to dodge attacks, avoiding damage completely. "));
	
	GameplayTags.Attributes_Secondary_MovementSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MovementSpeed"),
			FString("Increases the character's speed of movement, aiding in both combat and exploration. "));
	
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"),
			FString("Increases the rate at which health regenerates over time, aiding in recovery outside of combat. "));
	
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"),
			FString("Increases the rate at which mana regenerates over time, ensuring a steady supply of mana for spells. "));
	
	GameplayTags.Attributes_Secondary_StaminaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.StaminaRegeneration"),
			FString("Increases the rate at which stamina regenerates over time, aiding in recovery outside of combat. "));
	
	GameplayTags.Attributes_Secondary_MaximumHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaximumHealth"),
			FString("Increases the total amount of health. "));
	
	GameplayTags.Attributes_Secondary_MaximumMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaximumMana"),
			FString("Increases the total amount of mana, providing a larger pool for casting spells. "));
	
	GameplayTags.Attributes_Secondary_MaximumStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaximumStamina"),
			FString("Increases the total amount of stamina, providing a larger pool for physical activities and abilities. "));

	/*
	 * Input Tags
	 */

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"),
			FString("Input Tag for Left Mouse Button "));
	
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button "));
	
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"),
		FString("Input Tag for 1 key "));

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"),
		FString("Input Tag for 2 key "));
	
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"),
		FString("Input Tag for 3 key "));

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"),
		FString("Input Tag for 4 key "));

	/*
	 *
	 */

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"),
	FString("Damage"));

	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"),
	FString("Tag granted when Hit Reacting"));
}
