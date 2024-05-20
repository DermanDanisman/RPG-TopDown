// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"

AEnemyCharacter::AEnemyCharacter()
{
}

void AEnemyCharacter::HighlightActor()
{
	// TODO: Remove Later!
	FString ActorName;
	GetName(ActorName);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("EnemyCharacter %s: HighlightActor Called"), *ActorName));
}

void AEnemyCharacter::UnHighlightActor()
{
	// TODO: Remove Later!
	FString ActorName;
	GetName(ActorName);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("EnemyCharacter %s: UnHighlightActor Called"), *ActorName));
}
