// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	/** Widget Controller */
	// The way we're setting this up, the widget controller could literally be any object that makes it little bit more versatile.
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
protected:
	// Whenever we set the widget controller for a given widget, we'll also call this function widget controller set.
	// And as soon as that happens, anything we want in the blueprint that we would like to do in response to the widget controller set,
	// we'll do that in the blueprint.
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
