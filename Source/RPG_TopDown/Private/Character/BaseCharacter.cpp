// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "RPG_TopDown/RPG_TopDown.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	// Created Weapon Mesh for all the characters derived from BaseCharacter
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	// Attached the Weapon Mesh to Character Mesh
	WeaponMesh->SetupAttachment(GetMesh(), WeaponAttachmentSocketName);
	// Setting collision to NoCollision of the WeaponMesh, so we can edit it in the blueprint
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ABaseCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

FVector ABaseCharacter::GetWeaponTipSocketLocation()
{
	check(WeaponMesh);
	return WeaponMesh->GetSocketLocation(WeaponTipSocketName);
}

UAnimMontage* ABaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

// This function applies a gameplay effect to the character itself.
void ABaseCharacter::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const
{
	// Ensure that the AbilitySystemComponent and GameplayEffectClass are valid
	check(IsValid(GetAbilitySystemComponent()))
	check(GameplayEffectClass)
    
	// Create a context handle for the gameplay effect
	// Context Handle: Contains contextual information about the gameplay effect, such as its source and target.
	FGameplayEffectContextHandle GameplayEffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	
	// Because the object itself, which will be our character in this case, or in the case of the enemy,
	// the source object is the class that has the implemented interface function get player level from our combat interface.
	// So that's why we need to use the character itself.
	GameplayEffectContextHandle.AddSourceObject(this);
    
	// Create a specification handle for the gameplay effect
	// Spec Handle: Specifies the details of the gameplay effect, such as its magnitude, duration, and context.
	const FGameplayEffectSpecHandle GameplayEffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, GameplayEffectContextHandle);
    
	// Apply the gameplay effect to the target, which in this case is the character itself
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ABaseCharacter::AddCharacterAbilities() const
{
	UBaseAbilitySystemComponent* BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent);

	// This check ensures that only the server (or the authoritative instance) can add abilities.
	// In multiplayer games, this prevents clients from adding abilities arbitrarily.
	if (!HasAuthority()) return;

	// Calls the function on the Ability System Component to add the startup abilities.
	BaseAbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
}


void ABaseCharacter::InitAbilityActorInfo()
{
	
}

// This function initializes the character's default attributes by applying primary and secondary attribute effects to the character.
void ABaseCharacter::InitializeDefaultAttributes() const
{
	
}
