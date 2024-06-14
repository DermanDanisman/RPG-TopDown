// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TopDownAssetManager.generated.h"

/*
 * What is the Singleton Pattern?
 * The singleton pattern is a design pattern that restricts the instantiation of a class to one "single" instance.
 * This is useful when exactly one object is needed to coordinate actions across the system.
 */

/**
 * Purpose:
 * The TopDownAssetManager class is responsible for managing assets in the game, including the initialization of gameplay tags.
 * It ensures that all necessary assets and tags are loaded properly at the start of the game.
 *
 * Implementation:
 * Singleton Pattern: Similar to TopDownGameplayTags, this class uses the singleton pattern to maintain a single instance throughout the game.
 *
 * Initialization: It overrides the StartInitialLoading method to perform any necessary initialization tasks when the game starts, such as loading gameplay tags.
 *
 * Asset Management: The class can be extended to manage other assets required by the game, ensuring a centralized and efficient asset management system.
 */
UCLASS()
class RPG_TOPDOWN_API UTopDownAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	// Static method to get the single instance of the class
	static UTopDownAssetManager& Get();

protected:

	// Override to perform initial loading tasks
	virtual void StartInitialLoading() override;
};
