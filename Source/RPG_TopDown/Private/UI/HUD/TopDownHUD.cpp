// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/TopDownHUD.h"

#include "Blueprint/UserWidget.h"

void ATopDownHUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	Widget->AddToViewport();
}
