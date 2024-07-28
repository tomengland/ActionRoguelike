// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;
class UParticleSystem;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

	void PostInitializeComponents();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ExplodeAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundBase* FlightSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundAttenuation* ProjectileAttenuation;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SPlaySound(USoundBase* Sound, USoundAttenuation* AttenuationType);

	UFUNCTION()
	void OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
