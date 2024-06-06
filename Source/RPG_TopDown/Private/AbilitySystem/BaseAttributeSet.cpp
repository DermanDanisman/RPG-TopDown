// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

	/*
	 * GAMEPLAYATTRIBUTE_REPNOTIFY macro handles the boilerplate code for notifying clients of attribute changes.
	 * It ensures that the change in the attribute is properly replicated and triggers any bound delegates.
	 * GAMEPLAYATTRIBUTE_REPNOTIFY macro ensures the attribute change is properly replicated and triggers any bound delegates.
	 * Now when we set up the rep notify for a replicated attribute in an attribute set, we have to inform the ability system of that change.
	 * So the ability system can do all the under-the-hood bookkeeping that it needs to do to keep the ability system together working cohesively.
	 */
	

UBaseAttributeSet::UBaseAttributeSet()
{
	/*
	 * Even though I didn't create an init health function,
	 * I used my attribute accessors macro in header file which calls these four macros,
	 * one of which creates the INITTER and by default that's called init health.
	 */
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(25.f);
	InitMaxMana(50.f);
	InitStamina(50.f);
	InitMaxStamina(100.f);
}

/*
 * The GetLifetimeReplicatedProps function is used in Unreal Engine to define which properties of a class should be replicated over the network.
 * It is an essential part of the replication system,
 * ensuring that the state of certain properties is kept consistent between the server and the clients in a multiplayer game.
 */
void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*
	 * Primary Attributes
	 */
	// Setup replication for Strength attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	
	// Setup replication for Agility attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Agility, COND_None, REPNOTIFY_Always);

	// Setup replication for Intelligence attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);

	// Setup replication for Endurance attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Endurance, COND_None, REPNOTIFY_Always);

	// Setup replication for Charisma attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Charisma, COND_None, REPNOTIFY_Always);

	// Setup replication for Charisma attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Wisdom, COND_None, REPNOTIFY_Always);
	
	/*
	 * Vital Attributes
	 */
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

/*
 * PreAttributeBaseChange is called for instant and periodic effects that change the base value.
 * It is triggered by changes to Attributes.
 */
void UBaseAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}

/*
 * PreAttributeChange is called in all cases and is used to react to final attribute value changes.
 */
void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, NewValue));
	}	
	if (Attribute == GetMaxManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, NewValue));
	}
}

/*
 * The function is responsible for setting up and populating the FGameplayEffectExecutionContext struct.
 */
void UBaseAttributeSet::InitializeEffectExecutionContext(const FGameplayEffectModCallbackData& Data, FGameplayEffectContextDetails& GameplayEffectContextDetails) const
{
	// Initialize the context details struct 
	GameplayEffectContextDetails.GameplayEffectContextHandle = MakeShared<FGameplayEffectContextHandle>(Data.EffectSpec.GetContext());

	// Populate source properties
	GameplayEffectContextDetails.SourceProperties->AbilitySystemComponent = GameplayEffectContextDetails.GameplayEffectContextHandle->GetOriginalInstigatorAbilitySystemComponent();
	
	if (IsValid(GameplayEffectContextDetails.SourceProperties->AbilitySystemComponent) &&
		GameplayEffectContextDetails.SourceProperties->AbilitySystemComponent->AbilityActorInfo.IsValid() &&
		GameplayEffectContextDetails.SourceProperties->AbilitySystemComponent->AbilityActorInfo->AvatarActor.IsValid())
	{
		GameplayEffectContextDetails.SourceProperties->AvatarActor = GameplayEffectContextDetails.SourceProperties->AbilitySystemComponent->AbilityActorInfo->AvatarActor.Get();
		GameplayEffectContextDetails.SourceProperties->Controller = GameplayEffectContextDetails.SourceProperties->AbilitySystemComponent->AbilityActorInfo->PlayerController.Get();
		if (GameplayEffectContextDetails.SourceProperties->Controller == nullptr && GameplayEffectContextDetails.SourceProperties->AvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(GameplayEffectContextDetails.SourceProperties->AvatarActor))
			{
				GameplayEffectContextDetails.SourceProperties->Controller = Pawn->GetController();
			}
		}
		if (GameplayEffectContextDetails.SourceProperties->Controller)
		{
			GameplayEffectContextDetails.SourceProperties->Character = Cast<ACharacter>(GameplayEffectContextDetails.SourceProperties->Controller->GetPawn());
		}
	}

	// Populate target properties
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		GameplayEffectContextDetails.TargetProperties->AvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		GameplayEffectContextDetails.TargetProperties->Controller = Data.Target.AbilityActorInfo->PlayerController.Get();
		GameplayEffectContextDetails.TargetProperties->Character = Cast<ACharacter>(GameplayEffectContextDetails.TargetProperties->AvatarActor);
		GameplayEffectContextDetails.TargetProperties->AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GameplayEffectContextDetails.TargetProperties->AvatarActor);
	}

	// Now you can use EffectContextDetails to access all the gathered information
}

/*
 * This is executed after a gameplay effect changes an attribute, and we have access to a lot of information via this data input parameter.
 * We can access a lot of info based on the effect that was just applied, and we're going to harvest some information from this data parameter.
 * So ultimately, in PostGameplayEffectExecute, we have access to just about every entity involved in this gameplay effect being executed
 * PostGameplayEffectExecute is called after effect execution, primarily for instant and periodic effects.
 */
void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// The function is responsible for setting up and populating the FGameplayEffectExecutionContext struct.
	FGameplayEffectContextDetails GameplayEffectContextDetails;
	InitializeEffectExecutionContext(Data, GameplayEffectContextDetails);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
			FString::Printf(TEXT("Health: %f!"), GetHealth()));
	}
}

/*
 * Vital Attributes
 */

/*
 * The OnRep_Health function is called on the client when the Health attribute is updated on the server.
 * The GAMEPLAYATTRIBUTE_REPNOTIFY macro ensures the attribute is correctly replicated.
 * This function also adds a debug message to display the old and new Health values.
 */
void UBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health, OldHealth);
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
       // FString::Printf(TEXT("Health has been updated from %f to %f!"), OldHealth.GetCurrentValue(), Health.GetCurrentValue()));
}

/*
 * The OnRep_MaxHealth function is called on the client when the MaxHealth attribute is updated on the server.
 * The GAMEPLAYATTRIBUTE_REPNOTIFY macro ensures the attribute is correctly replicated.
 * This function also adds a debug message to display the old and new MaxHealth values.
 */
void UBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldMaxHealth);
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
     //   FString::Printf(TEXT("MaxHealth has been updated from %f to %f!"), OldMaxHealth.GetCurrentValue(), MaxHealth.GetCurrentValue()));
}

/*
 * The OnRep_Mana function is called on the client when the Mana attribute is updated on the server.
 * The GAMEPLAYATTRIBUTE_REPNOTIFY macro ensures the attribute is correctly replicated.
 * This function also adds a debug message to display the old and new Mana values.
 */
void UBaseAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Mana, OldMana);
   // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
     //   FString::Printf(TEXT("Mana has been updated from %f to %f!"), OldMana.GetCurrentValue(), Mana.GetCurrentValue()));
}

/*
 * The OnRep_MaxMana function is called on the client when the MaxMana attribute is updated on the server.
 * The GAMEPLAYATTRIBUTE_REPNOTIFY macro ensures the attribute is correctly replicated.
 * This function also adds a debug message to display the old and new MaxMana values.
 */
void UBaseAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxMana, OldMaxMana);
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
     //   FString::Printf(TEXT("MaxMana has been updated from %f to %f!"), OldMaxMana.GetCurrentValue(), MaxMana.GetCurrentValue()));
}

/*
 * The OnRep_Stamina function is called on the client when the Stamina attribute is updated on the server.
 * The GAMEPLAYATTRIBUTE_REPNOTIFY macro ensures the attribute is correctly replicated.
 * This function also adds a debug message to display the old and new MaxMana values.
 */
void UBaseAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Stamina, OldStamina);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
	//	FString::Printf(TEXT("MaxMana has been updated from %f to %f!"), OldStamina.GetCurrentValue(), Stamina.GetCurrentValue()));
}

/*
 * The OnRep_MaxStamina function is called on the client when the MaxStamina attribute is updated on the server.
 * The GAMEPLAYATTRIBUTE_REPNOTIFY macro ensures the attribute is correctly replicated.
 * This function also adds a debug message to display the old and new MaxMana values.
 */
void UBaseAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxStamina, OldMaxStamina);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
	//	FString::Printf(TEXT("MaxMana has been updated from %f to %f!"), OldMaxStamina.GetCurrentValue(), MaxStamina.GetCurrentValue()));
}


/*
 * Primary Attributes
 */
void UBaseAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Strength, OldStrength);
}

void UBaseAttributeSet::OnRep_Agility(const FGameplayAttributeData& OldAgility) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Agility, OldAgility);
}

void UBaseAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Intelligence, OldIntelligence);
}

void UBaseAttributeSet::OnRep_Endurance(const FGameplayAttributeData& OldEndurance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Endurance, OldEndurance);
}

void UBaseAttributeSet::OnRep_Charisma(const FGameplayAttributeData& OldCharisma) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Charisma, OldCharisma);
}

void UBaseAttributeSet::OnRep_Wisdom(const FGameplayAttributeData& OldWisdom) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Wisdom, OldWisdom);
}
