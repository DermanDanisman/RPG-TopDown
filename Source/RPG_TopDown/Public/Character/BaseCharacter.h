// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/Interaction/CombatInterface.h"
#include "BaseCharacter.generated.h"



/* Forward Declaration */
class UAttributeSet;
class UAbilitySystemComponent;
class UGameplayEffect;
class UGameplayAbility;


UCLASS()
class RPG_TOPDOWN_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	/*
	 * Ability System Interface
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;

	/*
	 * Combat Interface Functions
	 */
	virtual FVector GetWeaponTipSocketLocation() override;
	// We need to add Implementation to be able to override BlueprintImplementableEvent functions from Interfaces.  
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;

	/*
	 * Multicast RPC
	 */
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	
	/*
	 * Common Variables
	 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere,Category="Combat|Socket Names")
	FName WeaponTipSocketName;
	
	UPROPERTY(EditAnywhere,Category="Combat|Socket Names")
	FName WeaponAttachmentSocketName = FName("WeaponHandSocket");

	/*
	 * Game Ability System
	 */
	// This function is responsible for adding the abilities listed in StartupAbilities to the character's Ability System Component.
	void AddCharacterAbilities() const;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	/*
	 * Gameplay Effect Attributes
	 */
	// This function applies a gameplay effect to the character itself.
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const;
	
	// This function initializes the character's default attributes by applying primary and secondary attribute effects to the character.
	virtual void InitializeDefaultAttributes() const;
	
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes|Primary")
	//TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	// Initialize Secondary Attributes after the Primary Attributes become Secondary Attributes are dependent on Primary Attributes
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes|Secondary")
	//TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	// Initialize Vital Attributes after the Primary Attributes become Secondary Attributes are dependent on Primary Attributes
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes|Vital")
	//TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	/*
	 * Dissolve Effects
	 */
	void DissolveEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void StartCharacterMeshDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponMeshDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	
private:

	/*
	 * Gameplay Abilities
	 */
	// This is an array that holds classes derived from UGameplayAbility. These are the abilities that the character will start with.
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	/*
	 * Anim Montage
	 */
	UPROPERTY(EditAnywhere, Category="Combat|Anim Montage")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
