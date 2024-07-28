// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SBasePickup.generated.h"

class UStaticMeshComponent;
class USInteractionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASBasePickup : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASBasePickup();

	void Interact_Implementation(APawn* InstigatorPawn) override;

	

	void DisableInteraction();
	void EnableInteraction();




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bCanInteract = true;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	void PrimaryInteract();





public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
