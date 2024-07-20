// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TopDownAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "Controller/Widget/AttributeMenuWidgetController.h"
#include "Controller/Widget/BaseWidgetController.h"
#include "Game/TopDownGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerState/TopDownPlayerState.h"
#include "UI/HUD/TopDownHUD.h"

// Retrieves the overlay widget controller from the HUD associated with the player controller.
UOverlayWidgetController* UTopDownAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	// Get the first player controller in the world
	APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		// Get the HUD associated with the player controller
		ATopDownHUD* TopDownHUD = PC->GetHUD<ATopDownHUD>();
		if (TopDownHUD)
		{
			// Get the player state and its components
			ATopDownPlayerState* PS = PC->GetPlayerState<ATopDownPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerVariables WidgetControllerVariables(PC, PS, ASC, AS);
			// Return the overlay widget controller from the HUD
			return TopDownHUD->GetOverlayWidgetController(WidgetControllerVariables);
		}
	}
	return nullptr;
}

// We call this in WBP_AttributeMenu
// Retrieves the attribute menu widget controller from the HUD associated with the player controller.
UAttributeMenuWidgetController* UTopDownAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	// Get the first player controller in the world
	APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		// Get the HUD associated with the player controller
		ATopDownHUD* TopDownHUD = PC->GetHUD<ATopDownHUD>();
		if (TopDownHUD)
		{
			// Get the player state and its components
			ATopDownPlayerState* PS = PC->GetPlayerState<ATopDownPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerVariables WidgetControllerVariables(PC, PS, ASC, AS);
			// Return the attribute menu widget controller from the HUD
			return TopDownHUD->GetAttributeMenuWidgetController(WidgetControllerVariables);
		}
	}
	return nullptr;
}

// Initializes default attributes for a character based on their class and level.
void UTopDownAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, const ECharacterClass CharacterClass,
	const float Level, UAbilitySystemComponent* AbilitySystemComponent)
{
	// Get the avatar actor associated with the ability system component
	const AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();
	
	if (GetCharacterClassInfoDataAsset(WorldContextObject) == nullptr) return;
	
	// Get the character class info data asset from the game mode
	UCharacterClassInfoDataAsset* CharacterClassInfoDataAsset = GetCharacterClassInfoDataAsset(WorldContextObject);
	const FCharacterClassDefaultInfo CharacterClassDefaultInfoStruct = CharacterClassInfoDataAsset->GetCharacterClassDefaultInfo(CharacterClass);

	// Initialize primary attributes
	FGameplayEffectContextHandle PrimaryAttributesGameplayEffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	PrimaryAttributesGameplayEffectContextHandle.AddSourceObject(AvatarActor);

	const FGameplayEffectSpecHandle PrimaryAttributesGameplayEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CharacterClassDefaultInfoStruct.PrimaryAttributes,
		Level, PrimaryAttributesGameplayEffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesGameplayEffectSpecHandle.Data.Get());

	// Initialize secondary attributes
	FGameplayEffectContextHandle SecondaryAttributesGameplayEffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	SecondaryAttributesGameplayEffectContextHandle.AddSourceObject(AvatarActor);

	const FGameplayEffectSpecHandle SecondaryAttributesGameplayEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CharacterClassInfoDataAsset->SecondaryAttributes,
		Level, SecondaryAttributesGameplayEffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesGameplayEffectSpecHandle.Data.Get());

	// Initialize vital attributes
	FGameplayEffectContextHandle VitalAttributesGameplayEffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	VitalAttributesGameplayEffectContextHandle.AddSourceObject(AvatarActor);

	const FGameplayEffectSpecHandle VitalAttributesGameplayEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CharacterClassInfoDataAsset->VitalAttributes,
		Level, VitalAttributesGameplayEffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalAttributesGameplayEffectSpecHandle.Data.Get());
}

void UTopDownAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject,
	UAbilitySystemComponent* AbilitySystemComponent)
{
	if (GetCharacterClassInfoDataAsset(WorldContextObject) == nullptr) return;
	
	// Get the character class info data asset from the game mode
	UCharacterClassInfoDataAsset* CharacterClassInfoDataAsset = GetCharacterClassInfoDataAsset(WorldContextObject);
	for (const TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfoDataAsset->CommonGameplayAbilities)
	{
		// Create a gameplay ability spec and give the ability to the ability system component
		FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(AbilityClass, 1.f);
		AbilitySystemComponent->GiveAbility(GameplayAbilitySpec);
	}
}

UCharacterClassInfoDataAsset* UTopDownAbilitySystemLibrary::GetCharacterClassInfoDataAsset(
	const UObject* WorldContextObject)
{
	// Get the game mode and cast it to ATopDownGameModeBase
	const ATopDownGameModeBase* TopDownGameModeBase = Cast<ATopDownGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (TopDownGameModeBase == nullptr) return nullptr;

	// Get the character class info data asset from the game mode
	return TopDownGameModeBase->CharacterClassInfoDataAsset;
}

bool UTopDownAbilitySystemLibrary::GetIsEvaded(const FGameplayEffectContextHandle& GameplayEffectContextHandle)
{
	if (const FTopDownGameplayEffectContext* TopDownGameplayEffectContext = static_cast<const FTopDownGameplayEffectContext*>(GameplayEffectContextHandle.Get()))
	{
		return TopDownGameplayEffectContext->GetIsEvaded();
	}
	return false;
}

bool UTopDownAbilitySystemLibrary::GetIsCriticalHit(const FGameplayEffectContextHandle& GameplayEffectContextHandle)
{
	if (const FTopDownGameplayEffectContext* TopDownGameplayEffectContext = static_cast<const FTopDownGameplayEffectContext*>(GameplayEffectContextHandle.Get()))
	{
		return TopDownGameplayEffectContext->GetIsCriticalHit();
	}
	return false;
}

bool UTopDownAbilitySystemLibrary::GetIsBlockedHit(const FGameplayEffectContextHandle& GameplayEffectContextHandle)
{
	if (const FTopDownGameplayEffectContext* TopDownGameplayEffectContext = static_cast<const FTopDownGameplayEffectContext*>(GameplayEffectContextHandle.Get()))
	{
		return TopDownGameplayEffectContext->GetIsBlockedHit();
	}
	return false;
}

void UTopDownAbilitySystemLibrary::SetIsEvaded(FGameplayEffectContextHandle& GameplayEffectContextHandle, bool bInIsEvaded)
{
	if (FTopDownGameplayEffectContext* TopDownGameplayEffectContext = static_cast<FTopDownGameplayEffectContext*>(GameplayEffectContextHandle.Get()))
	{
		TopDownGameplayEffectContext->SetIsEvaded(bInIsEvaded);
	}
}

void UTopDownAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& GameplayEffectContextHandle, bool bInIsCriticalHit)
{
	if (FTopDownGameplayEffectContext* TopDownGameplayEffectContext = static_cast<FTopDownGameplayEffectContext*>(GameplayEffectContextHandle.Get()))
	{
		TopDownGameplayEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UTopDownAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& GameplayEffectContextHandle,
	bool bInIsBlockedHit)
{
	if (FTopDownGameplayEffectContext* TopDownGameplayEffectContext = static_cast<FTopDownGameplayEffectContext*>(GameplayEffectContextHandle.Get()))
	{
		TopDownGameplayEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}
