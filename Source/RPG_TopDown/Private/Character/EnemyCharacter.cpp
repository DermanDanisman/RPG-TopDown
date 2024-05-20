// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"

#include "RPG_TopDown/RPG_TopDown.h"

AEnemyCharacter::AEnemyCharacter()
{
}

void AEnemyCharacter::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	WeaponMesh->SetRenderCustomDepth(true);
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	
	// TODO: Remove Later!
	FString ActorName;
	GetName(ActorName);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("EnemyCharacter %s: HighlightActor Called"), *ActorName));
}

void AEnemyCharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	WeaponMesh->SetRenderCustomDepth(false);
	
	// TODO: Remove Later!
	FString ActorName;
	GetName(ActorName);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("EnemyCharacter %s: UnHighlightActor Called"), *ActorName));
}
