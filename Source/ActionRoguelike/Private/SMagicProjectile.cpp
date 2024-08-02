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
	
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComp");
	ProjectileComp->ProjectileGravityScale = 0;
	ProjectileComp->InitialSpeed = 3500.0f;
	ProjectileComp->bRotationFollowsVelocity = true;
	ProjectileComp->bInitialVelocityInLocalSpace = true;
	ExplodeAsset = nullptr;

}
void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
	SphereComp->OnComponentHit.AddDynamic(this, &ASMagicProjectile::OnSphereHit);
}




// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	SPlaySound(FlightSound, ProjectileAttenuation);
	
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetInstigator() && ExplodeAsset)
	{

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeAsset, (OtherActor->GetActorLocation()));
		if (OtherActor)
		{
			USAttributesComponent* AttributeComp = Cast<USAttributesComponent>(OtherActor->GetComponentByClass(USAttributesComponent::StaticClass()));

			if (AttributeComp)
			{

				AttributeComp->ApplyHealthChange(-20.0f);

				SPlaySound(ImpactSound, ProjectileAttenuation);

				
				Destroy();
			}
		}
		SPlaySound(ImpactSound, ProjectileAttenuation);
		Destroy();
	}

}

void ASMagicProjectile::SPlaySound(USoundBase* Sound, USoundAttenuation* AttenuationType)
{
	if (Sound && AttenuationType)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation(), 1.0f, 1.0f, 0.0f, AttenuationType);
	}
}

void ASMagicProjectile::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != GetInstigator() && ExplodeAsset)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeAsset, GetActorLocation(), GetActorRotation());
		SPlaySound(ImpactSound, ProjectileAttenuation);
		Destroy();
	}
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

