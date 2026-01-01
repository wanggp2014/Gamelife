// Fill out your copyright notice in the Description page of Project Settings.

#include "WgpPawn.h"
#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "Components/SphereComponent.h"

// Sets default values
AWgpPawn::AWgpPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	capsule->SetCapsuleHalfHeight(96.0f);
	capsule->SetCapsuleRadius(32.0f);
	SetRootComponent(capsule);

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	skeletalMesh->SetupAttachment(capsule);

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(capsule);
	springArm->TargetArmLength = 200.0f;
	springArm->ProbeSize = 18.f;

	viewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	viewCamera->SetupAttachment(springArm);

	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	sphereComponent->SetupAttachment(capsule);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AWgpPawn::BeginPlay()
{
	Super::BeginPlay();

	sphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AWgpPawn::OnSphereBeginOverlap_WGP);
	sphereComponent->OnComponentEndOverlap.AddDynamic(this, &AWgpPawn::OnSphereEndOverlap_WGP);
}

void AWgpPawn::MoveForward(float in_value)
{
	float value = 50.f * in_value;

	if (Controller && (value != 0.f))
	{
		FVector forward = GetActorForwardVector();
		AddMovementInput(forward, value);

		UE_LOG(LogTemp, Warning, TEXT("MoveForward: %f"), value);
	}
}

void AWgpPawn::Turn(float value)
{
	AddControllerYawInput(value);
	bUseControllerRotationYaw = true;
}

void AWgpPawn::Lookup(float value)
{
	AddControllerPitchInput(value);
	bUseControllerRotationPitch = true;
}

// Called every frame
void AWgpPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AWgpPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("Move Forward By WGP"), this, &AWgpPawn::MoveForward);
	PlayerInputComponent->BindAxis(FName("Turn By WGP"), this, &AWgpPawn::Turn);
	PlayerInputComponent->BindAxis(FName("Lookup By WGP"), this, &AWgpPawn::Lookup);
}

void AWgpPawn::OnSphereBeginOverlap_WGP(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AWgpPawn::OnSphereEndOverlap_WGP(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
