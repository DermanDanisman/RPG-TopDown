// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TopDownHUD.generated.h"

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
	UPROPERTY()
	TObjectPtr<UBaseUserWidget> OverlayWidget;
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OverlayWidgetClass;
};
