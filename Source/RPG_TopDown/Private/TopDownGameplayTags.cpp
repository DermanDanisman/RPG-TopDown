// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownGameplayTags.h"
#include "GameplayTagsManager.h"

FTopDownGameplayTags FTopDownGameplayTags::GameplayTags;

void FTopDownGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"),
		FString("Provides physical damage mitigation, reducing the damage taken from physical attacks. "));

	
}
