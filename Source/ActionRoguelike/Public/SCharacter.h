// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USInteractionComponent;
struct FInputActionInstance;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UParticleSystem;
class USAttributesComponent;

UCLASS()

class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ASCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;


    FTimerHandle TimerHandle_PrimaryAttack;
    FTimerHandle TimerHandle_AlternateAttack;
    FTimerHandle TimerHandle_Teleport;
    FTimerHandle TimerHandle_Explode;
    FTimerHandle TimerHandle_TeleportActor;


    UPROPERTY(VisibleAnywhere)
    UCameraComponent* CameraComp;

    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere)
    USInteractionComponent* InteractionComp;

    UPROPERTY(EditAnywhere, Category="Attack")
    TSubclassOf<AActor> TeleportClass;

    UPROPERTY(EditAnywhere, Category="Attack")
    TSubclassOf<AActor> PrimaryAttackClass;

    UPROPERTY(EditAnywhere, Category="Attack")
    TSubclassOf<AActor> AlternateAttackClass;

    UPROPERTY(EditAnywhere, Category="Attack")
    UAnimMontage* AttackAnim;


    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputMappingContext* DefaultInputMapping;

    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputAction* Input_Move;

    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputAction* Input_PrimaryAttack;

    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputAction* Input_Turn;

    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputAction* Input_Jump;

    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputAction* Input_PrimaryInteract;

    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputAction* Input_AlternateAttack;

    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputAction* Input_Teleport;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USAttributesComponent* AttributeComp;


    void Move(const FInputActionInstance& Instance);
    void Turn(const FInputActionInstance& Instance);
    virtual void Jump() override;
    void StopJump();

    void PrimaryAttack();
    void AlternateAttack();
    void AlternateAttack_Elapsed();
    void PrimaryInteract();
    void PrimaryAttack_TimeElapsed();
    void Teleport();
    void Teleport_Elapsed();

    AActor* SpawnProjectile(TSubclassOf<AActor> ActorClass);

    void TeleportActor(AActor* TeleportProjectile, UParticleSystem* ParticleSystem);

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;


    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};