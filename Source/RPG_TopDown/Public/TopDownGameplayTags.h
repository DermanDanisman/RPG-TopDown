// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * TopDownGameplayTags
 * Singleton containing native GameplayTags
 * We're going to design this in a specific way with a singleton.
 * What you do is you create a static getter function, so you don't need an instance of the class to exist
 */
struct FTopDownGameplayTags
{
public:
 static const FTopDownGameplayTags& Get() { return GameplayTags; }
 static void InitializeNativeGameplayTags();

 FGameplayTag Attributes_Secondary_Armor;
protected:

private:
 // Now, for a static variable like this, we need to go into the CPP file and explicitly declare the type there.
 static FTopDownGameplayTags GameplayTags;
};
