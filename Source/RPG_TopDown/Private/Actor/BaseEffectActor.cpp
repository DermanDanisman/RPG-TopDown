// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BaseEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Components/SphereComponent.h"

// Sets default values
ABaseEffectActor::ABaseEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultSceneRoot);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(Mesh);
	
}

// Called when the game starts or when spawned
void ABaseEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseEffectActor::OnActorOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ABaseEffectActor::OnActorEndOverlap);
}

void ABaseEffectActor::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO: Change this to apply a Gameplay Effect. For now, Using const_cast as a hack!
	IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (ASCInterface)
	{
		const UBaseAttributeSet* BaseAttributeSet = Cast<UBaseAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UBaseAttributeSet::StaticClass()));
		UBaseAttributeSet* MutableBaseAttributeSet = const_cast<UBaseAttributeSet*>(BaseAttributeSet);
		MutableBaseAttributeSet->SetHealth(BaseAttributeSet->GetHealth() + 25);
		Destroy();
	}
}

void ABaseEffectActor::OnActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

