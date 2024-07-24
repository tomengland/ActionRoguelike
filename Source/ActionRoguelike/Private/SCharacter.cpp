// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationSystemTypes.h"
#include "SInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this,
	&ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);



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
	InputComp->BindAction(Input_Move, ETriggerEvent::Triggered, this,
	&ASCharacter::Move);
	InputComp->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered,
	this, &ASCharacter::PrimaryAttack);
	InputComp->BindAction(Input_Turn, ETriggerEvent::Triggered, this,
	&ASCharacter::Turn);
	InputComp->BindAction(Input_Jump, ETriggerEvent::Triggered, this,
	&ASCharacter::Jump);
	InputComp->BindAction(Input_Jump, ETriggerEvent::Completed, this,
	&ASCharacter::StopJump);
	InputComp->BindAction(Input_PrimaryInteract, ETriggerEvent::Triggered, this,
	&ASCharacter::PrimaryInteract);


}

void ASCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

}
