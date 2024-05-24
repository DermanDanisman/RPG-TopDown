// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/BaseUserWidget.h"

void UBaseUserWidget::SetWidgetController(UObject* InWidgetController)
{
	//checkf(InWidgetController, TEXT("Widget Controller Is Not VALID!"))
	if (IsValid(InWidgetController))
	{
		WidgetController = InWidgetController;
		// When this function is called, WidgetControllerSet will run
		WidgetControllerSet();
	}
}
