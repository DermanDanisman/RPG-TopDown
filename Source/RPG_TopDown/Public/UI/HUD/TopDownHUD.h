// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TopDownHUD.generated.h"



/* Forward Declaration */
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class UBaseUserWidget;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerVariables;

/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API ATopDownHUD : public AHUD
{
	GENERATED_BODY()

public:
	
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerVariables& WidgetControllerVariables);

	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerVariables& WidgetControllerVariables);

	void InitializeOverlayWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	
	/*
	 * Overlay Widget
	 */
	
	/*
	 * The overlay is the overall widget we'll see on the screen, and it's going to contain a bunch of little widgets inside of it.
	 */
	UPROPERTY(VisibleAnywhere, Category="Widget|Overlay")
	TObjectPtr<UBaseUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere, Category="Widget|Overlay")
	TSubclassOf<UUserWidget> OverlayWidgetClass;

	UPROPERTY(EditAnywhere, Category="Widget|Overlay")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY(VisibleAnywhere, Category="Widget|Overlay")
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	/*
	 * Attribute Menu Widget
	 */

	UPROPERTY(VisibleAnywhere, Category="Widget|Attribute Menu")
	TObjectPtr<UBaseUserWidget> AttributeMenuWidget;
	
	UPROPERTY(EditAnywhere, Category="Widget|Attribute Menu")
	TSubclassOf<UUserWidget> AttributeMenuWidgetClass;

	UPROPERTY(EditAnywhere, Category="Widget|Attribute Menu")
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
	
	UPROPERTY(VisibleAnywhere, Category="Widget|Attribute Menu")
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;


};
