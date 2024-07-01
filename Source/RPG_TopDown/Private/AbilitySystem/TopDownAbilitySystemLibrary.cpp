// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TopDownAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "Controller/Widget/AttributeMenuWidgetController.h"
#include "Controller/Widget/BaseWidgetController.h"
#include "Game/TopDownGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerState/TopDownPlayerState.h"
#include "UI/HUD/TopDownHUD.h"

UOverlayWidgetController* UTopDownAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		ATopDownHUD* TopDownHUD = PC->GetHUD<ATopDownHUD>();
		if (TopDownHUD)
		{
			ATopDownPlayerState* PS = PC->GetPlayerState<ATopDownPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerVariables WidgetControllerVariables(PC, PS, ASC, AS);
			return TopDownHUD->GetOverlayWidgetController(WidgetControllerVariables);
		}
	}

	return nullptr;
}

// We call this in WBP_AttributeMenu
UAttributeMenuWidgetController* UTopDownAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		ATopDownHUD* TopDownHUD = PC->GetHUD<ATopDownHUD>();
		if (TopDownHUD)
		{
			ATopDownPlayerState* PS = PC->GetPlayerState<ATopDownPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerVariables WidgetControllerVariables(PC, PS, ASC, AS);
			return TopDownHUD->GetAttributeMenuWidgetController(WidgetControllerVariables);
		}
	}
	
	return nullptr;
}

void UTopDownAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, const ECharacterClass CharacterClass,
	const float Level, UAbilitySystemComponent* AbilitySystemComponent)
{
	const ATopDownGameModeBase* TopDownGameModeBase = Cast<ATopDownGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (TopDownGameModeBase == nullptr) return;

	const AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();
	
	UCharacterClassInfoDataAsset* CharacterClassInfoDataAsset = TopDownGameModeBase->CharacterClassInfoDataAsset;
	const FCharacterClassDefaultInfo CharacterClassDefaultInfoStruct = CharacterClassInfoDataAsset->GetCharacterClassDefaultInfo(CharacterClass);
	
	// Primary Attributes
	FGameplayEffectContextHandle PrimaryAttributesGameplayEffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	PrimaryAttributesGameplayEffectContextHandle.AddSourceObject(AvatarActor);
	
	const FGameplayEffectSpecHandle PrimaryAttributesGameplayEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CharacterClassDefaultInfoStruct.PrimaryAttributes,
		Level, PrimaryAttributesGameplayEffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesGameplayEffectSpecHandle.Data.Get());
	
	// Secondary Attributes
	FGameplayEffectContextHandle SecondaryAttributesGameplayEffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	SecondaryAttributesGameplayEffectContextHandle.AddSourceObject(AvatarActor);
	
	const FGameplayEffectSpecHandle SecondaryAttributesGameplayEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CharacterClassInfoDataAsset->SecondaryAttributes,
	 Level, SecondaryAttributesGameplayEffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesGameplayEffectSpecHandle.Data.Get());

	// Vital Attributes
	FGameplayEffectContextHandle VitalAttributesGameplayEffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	VitalAttributesGameplayEffectContextHandle.AddSourceObject(AvatarActor);
	
	const FGameplayEffectSpecHandle VitalAttributesGameplayEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CharacterClassInfoDataAsset->VitalAttributes,
		Level, VitalAttributesGameplayEffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalAttributesGameplayEffectSpecHandle.Data.Get());
}
