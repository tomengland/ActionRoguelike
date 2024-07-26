// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "Projects.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SAttributesComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComp");
	ProjectileComp->ProjectileGravityScale = 0;
	ProjectileComp->InitialSpeed = 3500.0f;
	ProjectileComp->bRotationFollowsVelocity = true;
	ProjectileComp->bInitialVelocityInLocalSpace = true;

}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetInstigator())
	{
		UParticleSystem* ParticleSystem = Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), nullptr,
			TEXT(
				"/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Primary/FX/P_Gideon_Primary_HitWorld")));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, (OtherActor->GetActorLocation()));
		if (OtherActor)
		{
			USAttributesComponent* AttributeComp = Cast<USAttributesComponent>(OtherActor->GetComponentByClass(USAttributesComponent::StaticClass()));

			if (AttributeComp)
			{

				AttributeComp->ApplyHealthChange(-20.0f);

				Destroy();
			}
		}

		Destroy();
	}

}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

