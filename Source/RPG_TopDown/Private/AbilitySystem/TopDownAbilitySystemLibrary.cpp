// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TopDownAbilitySystemLibrary.h"

#include "Controller/Widget/AttributeMenuWidgetController.h"
#include "Controller/Widget/BaseWidgetController.h"
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
