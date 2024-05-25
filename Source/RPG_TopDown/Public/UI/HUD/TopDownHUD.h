// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TopDownHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
/* Forward Declaring */
struct FWidgetControllerVariables;
class UOverlayWidgetController;
class UBaseUserWidget;
/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API ATopDownHUD : public AHUD
{
	GENERATED_BODY()

public:
	/**
	* The overlay is the overall widget we'll see on the screen, and it's going to contain a bunch of little widgets inside of it.
	*/
	UPROPERTY(VisibleAnywhere, Category="Overlay Widget")
	TObjectPtr<UBaseUserWidget> OverlayWidget;

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerVariables& WidgetControllerVariables);

	void InitializeOverlayWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
private:
	UPROPERTY(EditAnywhere, Category="Overlay Widget")
	TSubclassOf<UUserWidget> OverlayWidgetClass;

	UPROPERTY(EditAnywhere, Category="Overlay Widget")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY(VisibleAnywhere, Category="Overlay Widget")
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;


};
