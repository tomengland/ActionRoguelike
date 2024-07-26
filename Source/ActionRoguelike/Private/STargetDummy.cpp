// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"
#include "SAttributesComponent.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComponent;

	AttributesComponent = CreateDefaultSubobject<USAttributesComponent>("AttributesComponent");

}

void ASTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributesComponent->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);
}

// Called when the game starts or when spawned
void ASTargetDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASTargetDummy::OnHealthChanged(USAttributesComponent* OwningComp, AActor* InstigatorActor, float NewHealth, float Delta)
{
	if (Delta < 0)
	{
		//Play Hit Animation
		//Play Hit Sound
		//Play Hit Particle
	MeshComponent->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}


}

