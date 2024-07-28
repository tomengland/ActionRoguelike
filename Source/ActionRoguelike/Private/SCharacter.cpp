// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "SAttributesComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmCompoonent");
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComp->SetupAttachment(SpringArmComponent);

    GetCharacterMovement()->bOrientRotationToMovement = true;

    bUseControllerRotationYaw = false;

    InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

    AttributeComp = CreateDefaultSubobject<USAttributesComponent>("AttributeComp");

}

void ASCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}



// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ASCharacter::Move(const FInputActionInstance& Instance)
{
    FRotator ControlRot = GetControlRotation();
    ControlRot.Pitch = 0.0f;
    ControlRot.Roll = 0.0f;

    // get value from input and convert to vector

    const FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();

    // Move Forward and Back
    AddMovementInput(ControlRot.Vector(), AxisValue.Y);

    // Move Right/Left

    const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
    AddMovementInput(RightVector, AxisValue.X);
}

void ASCharacter::Turn(const FInputActionInstance& Instance)
{
    const FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();
    // Turn Left and Right
    AddControllerYawInput(AxisValue.X);

    // Turn up and down
    AddControllerPitchInput(AxisValue.Y);
}

void ASCharacter::PrimaryAttack()
{
    // do stuff

    PlayAnimMontage(AttackAnim);

    USkeletalMeshComponent* MeshComp = FindComponentByClass<USkeletalMeshComponent>();

    if (MeshComp)
    {
        UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, MeshComp, FName("Muzzle_01"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
    }
    

    GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this,
                                    &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ASCharacter::AlternateAttack()
{
    // do stuff
    PlayAnimMontage(AttackAnim);

    GetWorldTimerManager().SetTimer(TimerHandle_AlternateAttack, this,
                                    &ASCharacter::AlternateAttack_Elapsed,
                                    0.2f);
}

void ASCharacter::AlternateAttack_Elapsed()
{
	SpawnProjectile(AlternateAttackClass);
}

void ASCharacter::Teleport()
{
    PlayAnimMontage(AttackAnim);
    GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this,
                                    &ASCharacter::Teleport_Elapsed,
                                    0.2f);
}

void ASCharacter::Teleport_Elapsed()
{
    AActor* Projectile = SpawnProjectile(TeleportClass);
    // need to move this into header / assign in bp.
    UParticleSystem* ParticleSystem = Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), nullptr,
                                                                             TEXT(
                                                                                 "/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Primary/FX/P_Gideon_Primary_HitWorld")));
    TeleportActor(Projectile, ParticleSystem);

}


AActor* ASCharacter::SpawnProjectile(TSubclassOf<AActor> ActorClass)
{
	FVector CameraLocation = CameraComp->GetComponentLocation();
	FVector CameraForwardVector = CameraComp->GetForwardVector();
	FHitResult CameraHit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FVector EndPoint = CameraLocation + (CameraForwardVector * 10000.0f);
	FRotator SpawnDirection;


	bool bHit = GetWorld()->LineTraceSingleByChannel(
		CameraHit, CameraLocation, EndPoint, ECC_Visibility, CollisionParams);

	// take impact point if there is a hit, otherwise just take the endtrace of the linetrace as the finalpoint.
	bHit
		? (SpawnDirection = UKismetMathLibrary::FindLookAtRotation(HandLocation, CameraHit.ImpactPoint))
		: (SpawnDirection = UKismetMathLibrary::FindLookAtRotation(HandLocation, CameraHit.TraceEnd));

	FTransform SpawnTM = FTransform(SpawnDirection, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

    return(GetWorld()->SpawnActor<AActor>(ActorClass, SpawnTM, SpawnParams));
}

void ASCharacter::TeleportActor(AActor* TeleportProjectile, UParticleSystem* ParticleSystem)
{
	if (ensureAlways(TeleportProjectile))
	{
        
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Explode, [this, ParticleSystem, TeleportProjectile]()
			{
                //if the projectile is not valid, do move actor
				if (TeleportProjectile == nullptr)
				{
					MoveIgnoreActorAdd(GetInstigator());
					this->TeleportTo(TeleportProjectile->GetActorLocation(), TeleportProjectile->GetActorRotation());  // da fuk am i doing here..
					MoveIgnoreActorRemove(GetInstigator());
					return;
				}
				FVector ProjectileLocation = TeleportProjectile->GetActorLocation();
				FRotator ProjectileRotation = TeleportProjectile->GetActorRotation();
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, ProjectileLocation);
				TeleportProjectile->Destroy();
				// set up another timer, use projectile location (pass by value since it's small ~12 bytes), this will be the code to teleport the player.
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_TeleportActor, [this, ProjectileLocation, ProjectileRotation]()
					{

						MoveIgnoreActorAdd(GetInstigator());
						this->TeleportTo(ProjectileLocation, ProjectileRotation);
						MoveIgnoreActorRemove(GetInstigator());

					}, 0.2f, false);
			}, 0.2f, false);
	}
}

void ASCharacter::OnHealthChanged(USAttributesComponent* OwningComp, AActor* InstigatorActor, float NewHealth, float Delta)
{
    if (Delta < 0.0f)
    {
      
		GetMesh()->SetScalarParameterValueOnMaterials("Speed", GetWorld()->TimeSeconds);
        GetMesh()->SetScalarParameterValueOnMaterials("UseHitFlash", 1.0f);
        GetWorld()->GetTimerManager().SetTimer(PulseTimerHandle, [this]()
            {
                GetMesh()->SetScalarParameterValueOnMaterials("UseHitFlash", 0.0f);
                GetWorld()->GetTimerManager().ClearTimer(PulseTimerHandle);

            }, 0.3f, false);

      
    }

    if (NewHealth <= 0.0f && Delta < 0.0f)
    {
        APlayerController* PC = Cast<APlayerController>(GetController());

        DisableInput(PC);
        if (GetCapsuleComponent())
        {
            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }
}

void ASCharacter::Jump()
{
    ACharacter::Jump();
}

void ASCharacter::StopJump()
{
    StopJumping();
}


// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    const APlayerController* PC = GetController<APlayerController>();
    const ULocalPlayer* LP = PC->GetLocalPlayer();

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    check(Subsystem);

    Subsystem->ClearAllMappings();

    Subsystem->AddMappingContext(DefaultInputMapping, 0);

    UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

    //General
    InputComp->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ASCharacter::Move);
    InputComp->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryAttack);
    InputComp->BindAction(Input_Turn, ETriggerEvent::Triggered, this, &ASCharacter::Turn);
    InputComp->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ASCharacter::Jump);
    InputComp->BindAction(Input_Jump, ETriggerEvent::Completed, this, &ASCharacter::StopJump);
    InputComp->BindAction(Input_PrimaryInteract, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryInteract);
    InputComp->BindAction(Input_AlternateAttack, ETriggerEvent::Triggered, this, &ASCharacter::AlternateAttack);
    InputComp->BindAction(Input_Teleport, ETriggerEvent::Triggered, this, &ASCharacter::Teleport);
}

void ASCharacter::PrimaryInteract()
{
    InteractionComp->PrimaryInteract();
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
    SpawnProjectile(PrimaryAttackClass);


}