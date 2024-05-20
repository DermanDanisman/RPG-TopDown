// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

#include "Components/CapsuleComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

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

