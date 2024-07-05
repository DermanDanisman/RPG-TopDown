// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "TopDownGameplayTags.h"
#include "Controller/Player/PlayerCharacterController.h"
#include "GameFramework/Character.h"
#include "Interface/Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
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
	
	// Setup replication for Dexterity attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);

	// Setup replication for Intelligence attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);

	// Setup replication for Resilience attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	
	// Setup replication for Vigor attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	/*
	 * Secondary Attributes
	 */
	// Setup replication for AttackPower attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);

	// Setup replication for SpellPower attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, SpellPower, COND_None, REPNOTIFY_Always);

	// Setup replication for Armor attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Armor, COND_None, REPNOTIFY_Always);

	// Setup replication for MagicResistance attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MagicResistance, COND_None, REPNOTIFY_Always);

	// Setup replication for ArmorPenetration attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);

	// Setup replication for CriticalHitChance attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);

	// Setup replication for CriticalHitDamage attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);

	// Setup replication for CriticalHitResistance attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);

	// Setup replication for Evasion attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Evasion, COND_None, REPNOTIFY_Always);

	// Setup replication for MovementSpeed attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);

	// Setup replication for HealthRegeneration attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);

	// Setup replication for ManaRegeneration attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);

	// Setup replication for StaminaRegeneration attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, StaminaRegeneration, COND_None, REPNOTIFY_Always);

	// Setup replication for MaxHealth attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	// Setup replication for MaxMana attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	
	// Setup replication for MaxStamina attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	
	/*
	 * Vital Attributes
	 */
    // Setup replication for Health attribute
    DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Health, COND_None, REPNOTIFY_Always);
    
    // Setup replication for Mana attribute
    DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	
	// Setup replication for Stamina attribute
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
}

/*
 * PreAttributeBaseChange is called for instant and periodic effects that change the base value.
 * It is triggered by changes to Attributes.
 */
void UBaseAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	// Clamping Vital Attributes
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

	/*if (Attribute == GetMaxHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, NewValue));
	}	
	if (Attribute == GetMaxManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, NewValue));
	}
	if (Attribute == GetMaxStaminaAttribute())
	{
		SetMana(FMath::Clamp(GetStamina(), 0.f, NewValue));
	}*/
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

    // Clamping Vital Attributes: Ensures that the vital attributes (Health, Mana, Stamina) are within their valid ranges.
    
    // If the attribute modified by the gameplay effect is Health, clamp the health value between 0 and MaxHealth.
    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        // Clamps the Health value to ensure it does not go below 0 or above MaxHealth.
        SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
        // Logs the new Health value for debugging purposes.
        UE_LOG(LogTemp, Warning, TEXT("Changed Health on %s, Health: %f"), *GameplayEffectContextDetails.TargetProperties->AvatarActor->GetName(), GetHealth());
    }

    // If the attribute modified by the gameplay effect is Mana, clamp the Mana value between 0 and MaxMana.
    if (Data.EvaluatedData.Attribute == GetManaAttribute())
    {
        SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
    }

    // If the attribute modified by the gameplay effect is Stamina, clamp the Stamina value between 0 and MaxStamina.
    if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
    {
        SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
    }

    // If the attribute modified by the gameplay effect is IncomingDamage, handle the incoming damage.
    if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
    {
        // Store the current value of IncomingDamage and then reset it to 0.
        const float LocalIncomingDamage = GetIncomingDamage();
        SetIncomingDamage(0.f);

        // If there is any incoming damage to process, reduce the Health accordingly.
        if (LocalIncomingDamage > 0.f)
        {
            // Calculate the new Health value after taking damage.
            const float NewHealth = GetHealth() - LocalIncomingDamage;
            // Clamp the new Health value to ensure it does not go below 0 or above MaxHealth.
            SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

            // Determine if the damage was fatal (i.e., Health dropped to 0 or below).
            const bool bFatal = NewHealth <= 0.f;
            if (bFatal)
            {
            	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GameplayEffectContextDetails.TargetProperties->AvatarActor);
            	if (CombatInterface)
            	{
            		CombatInterface->Die();
            	}
            }
            else
            {
            	// If the damage was not fatal, trigger a hit reaction ability.
            	FGameplayTagContainer GameplayTagContainer;
            	GameplayTagContainer.AddTag(FTopDownGameplayTags::Get().Effects_HitReact);
            	// Try to activate any abilities associated with the hit reaction tag.
            	GameplayEffectContextDetails.TargetProperties->AbilitySystemComponent->TryActivateAbilitiesByTag(GameplayTagContainer);
            }
        	ShowFloatingDamageText(GameplayEffectContextDetails, LocalIncomingDamage);
        }
    }
}

void UBaseAttributeSet::ShowFloatingDamageText(const FGameplayEffectContextDetails& GameplayEffectContextDetails, const float Damage) const
{
	if (GameplayEffectContextDetails.SourceProperties->Character != GameplayEffectContextDetails.TargetProperties->Character)
	{
		if (APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(
			UGameplayStatics::GetPlayerController(GameplayEffectContextDetails.SourceProperties->Character, 0)))
		{
			PlayerCharacterController->ShowDamageNumber(Damage, GameplayEffectContextDetails.TargetProperties->Character);
		}
	}
}

/*
 * The OnRep_Health function is called on the client when the Health attribute is updated on the server.
 * The GAMEPLAYATTRIBUTE_REPNOTIFY macro ensures the attribute is correctly replicated.
 * This function also adds a debug message to display the old and new Health values.
 */

/*
 * Vital Attributes
 */
void UBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health, OldHealth);
}

void UBaseAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Mana, OldMana);
}

void UBaseAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Stamina, OldStamina);
}

/*
 * Primary Attributes
 */
void UBaseAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Strength, OldStrength);
}

void UBaseAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Dexterity, OldDexterity);
}

void UBaseAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Intelligence, OldIntelligence);
}

void UBaseAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Resilience, OldResilience);
}

void UBaseAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Vigor, OldVigor);
}

/*
 * Secondary Attributes
 */

void UBaseAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, AttackPower, OldAttackPower);
}

void UBaseAttributeSet::OnRep_SpellPower(const FGameplayAttributeData& OldSpellPower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, SpellPower, OldSpellPower);
}

void UBaseAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Armor, OldArmor);
}

void UBaseAttributeSet::OnRep_MagicResistance(const FGameplayAttributeData& OldMagicResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MagicResistance, OldMagicResistance);
}

void UBaseAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UBaseAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UBaseAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UBaseAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UBaseAttributeSet::OnRep_Evasion(const FGameplayAttributeData& OldEvasion) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Evasion, OldEvasion);
}

void UBaseAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MovementSpeed, OldMovementSpeed);
}

void UBaseAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UBaseAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UBaseAttributeSet::OnRep_StaminaRegeneration(const FGameplayAttributeData& OldStaminaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, StaminaRegeneration, OldStaminaRegeneration);
}

void UBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldMaxHealth);
}

void UBaseAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxMana, OldMaxMana);
}

void UBaseAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxStamina, OldMaxStamina);
}

