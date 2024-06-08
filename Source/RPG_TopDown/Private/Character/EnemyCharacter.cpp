// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "RPG_TopDown/RPG_TopDown.h"

AEnemyCharacter::AEnemyCharacter()
{
	// Create and initialize the AbilitySystemComponent
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystemComponent");
	// Set the AbilitySystemComponent to be replicated
	AbilitySystemComponent->SetIsReplicated(true);
	// Set the replication mode to Minimal
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	// Create and initialize the AttributeSet
	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Calling Ability System Init Ability Actor Info
	InitAbilityActorInfo();
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	
	/**
	 * InitAbilityActorInfo is a function typically used in the context of Unreal Engine's Gameplay Ability System (GAS).
	 * Its primary purpose is to initialize the ability-related information for an actor,
	 * such as setting up the AbilitySystemComponent and the associated AttributeSet.
	 * This function ensures that the actor has all the necessary components and information to use abilities and manage their attributes effectively.
	 */
	/**
	* Typical Responsibilities of InitAbilityActorInfo

	1. Retrieve and Set Player State and Controller References:
		It obtains references to the player state and player controller, which are essential for managing the actor's state and abilities.

	2. Initialize the Ability System Component:
		It sets up the AbilitySystemComponent, which is responsible for managing abilities and gameplay effects.

	3. Set the Attribute Set:
		It assigns the AttributeSet to manage various attributes like health, mana, strength, etc.

	4. Setup Initial Ability Info:
		It initializes the AbilitySystemComponent with the actor's information, linking it to the player state and the actor itself.

	5. Initialize any UI-related Components:
		It can also initialize any UI components related to the ability system, such as updating the HUD with the initial attribute values.
	 */
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	// Binding FOnGameplayEffectAppliedDelegate OnGameplayEffectAppliedDelegateToSelf delegate.
	Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent)->BindOnGameplayEffectAppliedDelegateToSelf();
}

//Highlight the actor by enabling custom depth rendering with a specific stencil value
void AEnemyCharacter::HighlightActor()
{
	// Enable custom depth rendering for the mesh
	GetMesh()->SetRenderCustomDepth(true);
	// Set the stencil value for custom depth rendering to red
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	// Enable custom depth rendering for the weapon mesh
	WeaponMesh->SetRenderCustomDepth(true);
	// Set the stencil value for the weapon mesh to red
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}


//Unhighlight the actor by disabling custom depth rendering 
void AEnemyCharacter::UnHighlightActor()
{
	// Disable custom depth rendering for the mesh
	GetMesh()->SetRenderCustomDepth(false);
	// Disable custom depth rendering for the weapon mesh
	WeaponMesh->SetRenderCustomDepth(false);
}

// Getting Enemy Character Level
int32 AEnemyCharacter::GetCharacterLevel()
{
	return Level;
}
