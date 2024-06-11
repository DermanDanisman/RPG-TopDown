// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TopDownAbilitySystemLibrary.h"

#include "Controller/Widget/AttributeMenuWidgetController.h"
#include "Controller/Widget/BaseWidgetController.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerState/TopDownPlayerState.h"
#include "UI/HUD/TopDownHUD.h"

UOverlayWidgetController* UTopDownAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController();

	if (PC)
	{
		if (ATopDownHUD* TopDownHUD = PC->GetHUD<ATopDownHUD>())
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

UAttributeMenuWidgetController* UTopDownAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);

	if (PC)
	{
		if (ATopDownHUD* TopDownHUD = PC->GetHUD<ATopDownHUD>())
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
