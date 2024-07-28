// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBasePickup.h"
#include "SHealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASHealthPickup : public ASBasePickup
{
	GENERATED_BODY()

public:
	ASHealthPickup();

	void Interact_Implementation(APawn* InstigatorPawn) override;

private:
	UPROPERTY(EditAnywhere, Category="Pickup")
	float HealthAmount;
	
	FTimerHandle InactiveTimer;
	
};
