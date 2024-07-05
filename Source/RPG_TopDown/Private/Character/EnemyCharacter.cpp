// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"

#include "TopDownGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/TopDownAbilitySystemLibrary.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RPG_TopDown/RPG_TopDown.h"
#include "UI/Widget/BaseUserWidget.h"

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

	// Create and initialize the HealthBar
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set WalkSpeed to BaseWalkSpeed
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;

	// Calling Ability System Init Ability Actor Info
	InitAbilityActorInfo();

	// Giving Startup Abilities to Enemy such as Hit React ability to play Hit React Montage.
	UTopDownAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);

	
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

	// Initializing Primary, Secondary and Vital Attributes.
	InitializeDefaultAttributes();
	
	// Setting Widget Controller to Enemy Class itself.
	if (UBaseUserWidget* BaseUserWidget = Cast<UBaseUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		BaseUserWidget->SetWidgetController(this);
	}

	/*
	 * Binding Lambda functions to Attribute Changes and Broadcasting OnHealthChanged and OnMaxHealthChanged.
	 */
	UBaseAttributeSet* BaseAttributeSet = Cast<UBaseAttributeSet>(AttributeSet);
	if (BaseAttributeSet)
	{
		// Bind callback for health attribute changes.
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		
		// Bind callback for max health attribute changes.
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		
		AbilitySystemComponent->RegisterGameplayTagEvent(FTopDownGameplayTags::Get().Effects_HitReact,
			EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AEnemyCharacter::HitReactTagChange);

		// Broadcasting initial values
		OnHealthChanged.Broadcast(BaseAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(BaseAttributeSet->GetMaxHealth());
	}
}

// Callback function to RegisterGameplayTagEvent when the enemy gets hit by a player
void AEnemyCharacter::HitReactTagChange(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void AEnemyCharacter::InitializeDefaultAttributes() const
{
	/*
	 * So now we're initializing the DefaultAttributes using our new function that we added to our UTopDownAbilitySystemLibrary
	 * that gets the CharacterClassInfoDataAsset stored on the TopDownGameModeBase, gets the class information based on our character's class,
	 * and applies a Gameplay Effect at the level passed in.
	 */
	UTopDownAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterCLass, Level, AbilitySystemComponent);
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
