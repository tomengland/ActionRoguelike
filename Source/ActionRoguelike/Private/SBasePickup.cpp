// Fill out your copyright notice in the Description page of Project Settings.


#include "SBasePickup.h"
#include "Components/StaticMeshComponent.h"
#include "SInteractionComponent.h"

// Sets default values
ASBasePickup::ASBasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;
	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

}



void ASBasePickup::DisableInteraction()
{
	bCanInteract = false;

}

void ASBasePickup::EnableInteraction()
{
	bCanInteract = true;
}

void ASBasePickup::Interact_Implementation(APawn* InstigatorPawn)
{
	

}

// Called when the game starts or when spawned
void ASBasePickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASBasePickup::PrimaryInteract()
{

}

// Called every frame
void ASBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

