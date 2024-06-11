// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Widget/OverlayWidgetController.h"

#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"


// Broadcasts the initial values of the attributes when the widget is first initialized.
void UOverlayWidgetController::BroadcastInitialValues()
{
	// Cast the AttributeSet to UBaseAttributeSet to access its methods.
	const UBaseAttributeSet* BaseAttributeSet = Cast<UBaseAttributeSet>(AttributeSet);
	
	// Broadcast the initial health value.
	OnHealthChanged.Broadcast(BaseAttributeSet->GetHealth());
	// Broadcast the initial max health value.
	OnMaxHealthChanged.Broadcast(BaseAttributeSet->GetMaxHealth());

	// Broadcast the initial mana value.
	OnManaChanged.Broadcast(BaseAttributeSet->GetMana());
	// Broadcast the initial max mana value.
	OnMaxManaChanged.Broadcast(BaseAttributeSet->GetMaxMana());

	// Broadcast the initial stamina value.
	OnStaminaChanged.Broadcast(BaseAttributeSet->GetStamina());
	// Broadcast the initial max stamina value.
	OnMaxStaminaChanged.Broadcast(BaseAttributeSet->GetMaxStamina());
}

// Function to set up the binding of attribute change callbacks
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// Cast the AttributeSet to UBaseAttributeSet to access its methods.
	const UBaseAttributeSet* BaseAttributeSet = Cast<UBaseAttributeSet>(AttributeSet);

	// Function to bind attribute change callbacks to the Ability System Component
	BindVitalAttributeChangeCallbacks(BaseAttributeSet);

	// Function to bind GameplayEffectAssetTags to a lambda function
	BindGameplayEffectTagMessages();
}

// Function to bind attribute change callbacks to the Ability System Component
void UOverlayWidgetController::BindVitalAttributeChangeCallbacks(const UBaseAttributeSet* BaseAttributeSet)
{
	/*
	 * What is AddLambda?
	 * AddLambda is a method used to bind a lambda function to a delegate in Unreal Engine.
	 * A lambda function is an anonymous function that can capture variables from its surrounding scope.
	 * It's a convenient way to define small, inline functions without having to write a separate function.
	 *
	 * How Does AddLambda Work?
	 * When you call AddLambda on a delegate, you pass in a lambda function that you want to bind to that delegate.
	 * The delegate then stores the lambda and calls it whenever the delegate is broadcasted.
	 *
	 * Why Use AddLambda?
 	 * Inline Functions: AddLambda allows you to define small functions inline where they are used, which can make your code more readable and localized.
	 * Capture Local Variables: You can capture local variables within the lambda, making it easier to access local state without passing additional parameters.
	 * Flexibility: It provides a flexible way to quickly bind behavior to delegates without needing to define separate callback functions.
	 * Reduced Boilerplate: Reduces the need to define multiple small callback functions and their associated boilerplate code.
	 *
	 * Why Use a Lambda Here?
	 * Localization: The lambda function is defined right where it's used, making it easy to see the relationship between the delegate and its handler.
	 * Simplicity: No need to define a separate member function if the logic is simple and used only in this context.
	 * Access to Local Variables: Although this example doesn't capture any local variables, lambdas can capture local state if needed, which can be very convenient.
	 *
	 * The square brackets are for captures. If we want to access a member variable from something, then that thing, that class that has the member variable must be captured within the lambda.
	 * Now we can capture variables, we can capture things by reference, we can capture things by pointer.
	 * So if you want to call a member function in a lambda, you have to capture that object of the class that that function belongs to.
	 */
	
	// Bind callback for health attribute changes.
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);


	// Bind callback for max health attribute changes.
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	// Bind callback for mana attribute changes.
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);


	// Bind callback for max mana attribute changes.
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
	
	// Bind callback for stamina attribute changes.
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnStaminaChanged.Broadcast(Data.NewValue);
		}
	);
	
	// Bind callback for max stamina attribute changes.
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxStaminaChanged.Broadcast(Data.NewValue);
		}
	);
}

// Function to bind GameplayEffectAssetTags to a lambda function
void UOverlayWidgetController::BindGameplayEffectTagMessages()
{
	/*
	 * What is AddLambda?
	 * AddLambda is a method used to bind a lambda function to a delegate in Unreal Engine.
	 * A lambda function is an anonymous function that can capture variables from its surrounding scope.
	 * It's a convenient way to define small, inline functions without having to write a separate function.
	 *
	 * How Does AddLambda Work?
	 * When you call AddLambda on a delegate, you pass in a lambda function that you want to bind to that delegate.
	 * The delegate then stores the lambda and calls it whenever the delegate is broadcasted.
	 *
	 * Why Use AddLambda?
 	 * Inline Functions: AddLambda allows you to define small functions inline where they are used, which can make your code more readable and localized.
	 * Capture Local Variables: You can capture local variables within the lambda, making it easier to access local state without passing additional parameters.
	 * Flexibility: It provides a flexible way to quickly bind behavior to delegates without needing to define separate callback functions.
	 * Reduced Boilerplate: Reduces the need to define multiple small callback functions and their associated boilerplate code.
	 *
	 * Why Use a Lambda Here?
	 * Localization: The lambda function is defined right where it's used, making it easy to see the relationship between the delegate and its handler.
	 * Simplicity: No need to define a separate member function if the logic is simple and used only in this context.
	 * Access to Local Variables: Although this example doesn't capture any local variables, lambdas can capture local state if needed, which can be very convenient.
	 *
	 * The square brackets are for captures. If we want to access a member variable from something, then that thing, that class that has the member variable must be captured within the lambda.
	 * Now we can capture variables, we can capture things by reference, we can capture things by pointer.
	 * So if you want to call a member function in a lambda, you have to capture that object of the class that that function belongs to.
	 */
	// Cast to UBaseAbilitySystemComponent and bind a lambda to the GameplayEffectAssetTags delegate
	Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent)->GameplayEffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			// Iterate over each GameplayTag in the container and print a message
			for (const FGameplayTag& Tag : AssetTags)
			{
				// For example, say that Tag = Message.HealthPotion
				// "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					const FGameplayTagMessageInfoRow* MessageInfoRow = GetDataTableRowByTag<FGameplayTagMessageInfoRow>(GameplayTagToUIMessageDataTable, Tag);
					if (MessageInfoRow)
					{
						GameplayTagMessageInfoRowDelegate.Broadcast(*MessageInfoRow);
					}
					//const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
					//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, Msg);
				}
			}
		}
	);
}
