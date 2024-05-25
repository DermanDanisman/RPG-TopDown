// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAttributeSet.h"
#include "Net/UnrealNetwork.h"

	/**
	 * GAMEPLAYATTRIBUTE_REPNOTIFY macro handles the boilerplate code for notifying clients of attribute changes.
	 * It ensures that the change in the attribute is properly replicated and triggers any bound delegates.
	 * GAMEPLAYATTRIBUTE_REPNOTIFY macro ensures the attribute change is properly replicated and triggers any bound delegates.
	 * Now when we set up the rep notify for a replicated attribute in an attribute set, we have to inform the ability system of that change.
	 * So the ability system can do all the under-the-hood bookkeeping that it needs to do to keep the ability system together working cohesively.
	 */
	

UBaseAttributeSet::UBaseAttributeSet()
{
	/**
	* Even though I didn't create an init health function,
	* I used my attribute accessors macro in header file which calls these four macros,
	* one of which creates the INITTER and by default that's called init health.
	*/
	InitHealth(75.f);
	InitMaxHealth(100.f);
	InitMana(25.f);
	InitMaxMana(50.f);
	InitStamina(50.f);
	InitMaxStamina(100.f);
}

/**
 * The GetLifetimeReplicatedProps function is used in Unreal Engine to define which properties of a class should be replicated over the network.
 * It is an essential part of the replication system,
 * ensuring that the state of certain properties is kept consistent between the server and the clients in a multiplayer game.
 */
void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    // Setup replication for Health attribute
    DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Health, COND_None, REPNOTIFY_Always);
    
    // Setup replication for MaxHealth attribute
    DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    
    // Setup replication for Mana attribute
    DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Mana, COND_None, REPNOTIFY_Always);
    
    // Setup replication for MaxMana attribute
    DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	// Setup replication for Stamina attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
    
	// Setup replication for MaxStamina attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

/**
 * The OnRep_Health function is called on the client when the Health attribute is updated on the server.
 * The GAMEPLAYATTRIBUTE_REPNOTIFY macro ensures the attribute is correctly replicated.
 * This function also adds a debug message to display the old and new Health values.
 */
void UBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health, OldHealth);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
        FString::Printf(TEXT("Health has been updated from %f to %f!"), OldHealth.GetCurrentValue(), Health.GetCurrentValue()));
}

/**
 * The OnRep_MaxHealth function is called on the client when the MaxHealth attribute is updated on the server.
 * The GAMEPLAYATTRIBUTE_REPNOTIFY macro ensures the attribute is correctly replicated.
 * This function also adds a debug message to display the old and new MaxHealth values.
 */
void UBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldMaxHealth);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
        FString::Printf(TEXT("MaxHealth has been updated from %f to %f!"), OldMaxHealth.GetCurrentValue(), MaxHealth.GetCurrentValue()));
}

/**
 * The OnRep_Mana function is called on the client when the Mana attribute is updated on the server.
 * The GAMEPLAYATTRIBUTE_REPNOTIFY macro ensures the attribute is correctly replicated.
 * This function also adds a debug message to display the old and new Mana values.
 */
void UBaseAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Mana, OldMana);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
        FString::Printf(TEXT("Mana has been updated from %f to %f!"), OldMana.GetCurrentValue(), Mana.GetCurrentValue()));
}

/**
 * The OnRep_MaxMana function is called on the client when the MaxMana attribute is updated on the server.
 * The GAMEPLAYATTRIBUTE_REPNOTIFY macro ensures the attribute is correctly replicated.
 * This function also adds a debug message to display the old and new MaxMana values.
 */
void UBaseAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxMana, OldMaxMana);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
        FString::Printf(TEXT("MaxMana has been updated from %f to %f!"), OldMaxMana.GetCurrentValue(), MaxMana.GetCurrentValue()));
}

/**
 * The OnRep_Stamina function is called on the client when the Stamina attribute is updated on the server.
 * The GAMEPLAYATTRIBUTE_REPNOTIFY macro ensures the attribute is correctly replicated.
 * This function also adds a debug message to display the old and new MaxMana values.
 */
void UBaseAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Stamina, OldStamina);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
		FString::Printf(TEXT("MaxMana has been updated from %f to %f!"), OldStamina.GetCurrentValue(), Stamina.GetCurrentValue()));
}

/**
 * The OnRep_MaxStamina function is called on the client when the MaxStamina attribute is updated on the server.
 * The GAMEPLAYATTRIBUTE_REPNOTIFY macro ensures the attribute is correctly replicated.
 * This function also adds a debug message to display the old and new MaxMana values.
 */
void UBaseAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxStamina, OldMaxStamina);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
		FString::Printf(TEXT("MaxMana has been updated from %f to %f!"), OldMaxStamina.GetCurrentValue(), MaxStamina.GetCurrentValue()));
}
