// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEffectActor.generated.h"

/** Forward Declaring Classes */
class USphereComponent;

UCLASS()
class RPG_TOPDOWN_API ABaseEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseEffectActor();

	/** Bind Overlap Functions */
	UFUNCTION()
	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
};
