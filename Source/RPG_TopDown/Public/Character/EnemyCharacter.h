// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfoDataAsset.h"
#include "Character/BaseCharacter.h"
#include "Interface/Interaction/HighlightActorInterface.h"
#include "Controller/Widget/OverlayWidgetController.h"
#include "EnemyCharacter.generated.h"

/* Forward Declaration */
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API AEnemyCharacter : public ABaseCharacter, public IHighlightActorInterface
{
	GENERATED_BODY()
	
public:
	
	AEnemyCharacter();

	/* Delegate Signature from Overlay Widget Controller */
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	/* Highlight Actor Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	/* Combat Interface */
	virtual int32 GetCharacterLevel() override;
	virtual void Die() override;
	virtual ECharacterClass GetCharacterClass() override;

	/* Hit React Callback */
	void HitReactTagChange(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;
	UPROPERTY(BlueprintReadOnly, Category="Combat")
	float BaseWalkSpeed = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float LifeSpan = 5.f;
	
protected:

	virtual void BeginPlay() override;


	/* Game Ability System */
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	/* Widget Initialization */
	void InitializeHealthBarWidgetController();
    
	/* Attribute Binding */
	void BindAndBroadcastAttributeChanges();
	
	/*
	 * Now the reason I'm not going to make this replicated for the enemy is because we're only going to be concerned with checking the level on the server for AI controlled enemies.
	 * And that's because important things that will require that level in calculations will only be done on the server.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	ECharacterClass CharacterCLass = ECharacterClass::Warrior;

	/* Widget Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
};
