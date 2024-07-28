// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPickup.h"
#include "SAttributesComponent.h"

ASHealthPickup::ASHealthPickup()
{

}

void ASHealthPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	USAttributesComponent* Attributes = InstigatorPawn->GetComponentByClass<USAttributesComponent>();

	if (Attributes->isAlreadyMaxHealth())
	{
		UE_LOG(LogTemp, Warning, TEXT("Already at max health"));
		return;
	}

	if (InstigatorPawn && bCanInteract)
	{
		if (Attributes)
		{
			Attributes->ApplyHealthChange(100.0f);
			MeshComp->SetVisibility(false);
			DisableInteraction();
		}
	}

	GetWorldTimerManager().SetTimer(InactiveTimer, [this]()
		{
			MeshComp->SetVisibility(true);
			EnableInteraction();

		}, 10.0f, false);

}


