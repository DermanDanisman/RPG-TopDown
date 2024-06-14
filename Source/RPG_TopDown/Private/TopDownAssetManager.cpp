// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownAssetManager.h"
#include "TopDownGameplayTags.h"

UTopDownAssetManager& UTopDownAssetManager::Get()
{
	check(GEngine->AssetManager)
	
	UTopDownAssetManager* TopDownAssetManager = Cast<UTopDownAssetManager>(GEngine->AssetManager);
	return *TopDownAssetManager;
}

void UTopDownAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	// Initialize native gameplay tags
	FTopDownGameplayTags::InitializeNativeGameplayTags();
}
