// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributesComponent.h"

// Sets default values for this component's properties
USAttributesComponent::USAttributesComponent()
{


	// ...
	Health = 100.0f;
	HealthMax = 100.0f;
}


bool USAttributesComponent::ApplyHealthChange(float Delta)
{

	Health = FMath::Clamp((Health + Delta), 0.0f, HealthMax);

	OnHealthChanged.Broadcast(this, nullptr, Health, Delta);

	return true;
}

bool USAttributesComponent::isAlive() const
{
	return Health > 0.0f;

}

bool USAttributesComponent::isAlreadyMaxHealth() const
{
	return (Health == HealthMax) ? true : false;
}

