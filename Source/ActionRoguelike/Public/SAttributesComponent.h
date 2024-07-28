// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributesComponent.generated.h"

// make custom event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, USAttributesComponent*, OwningComp, AActor*, InstigatorActor, float, NewHealth, float, Delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributesComponent();

protected:


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float HealthMax;

	// Stamina, Strength

public:	

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float Delta);

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool isAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool isAlreadyMaxHealth() const;

		
};
