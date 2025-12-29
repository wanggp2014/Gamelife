// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "WgpPawn.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent; 
class USphereComponent;

UCLASS()
class WGPOPENWORLD_API AWgpPawn : public APawn
{
    GENERATED_BODY()

public:
    AWgpPawn();

protected:
    virtual void BeginPlay() override;
    void MoveForward(float value);
    void Turn(float value);
    void Lookup(float value);

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION()
    void OnSphereBeginOverlap_WGP(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnSphereEndOverlap_WGP(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
    UPROPERTY(VisibleAnywhere)
    UCapsuleComponent* capsule;

    UPROPERTY(VisibleAnywhere)
    USkeletalMeshComponent* skeletalMesh;

    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* springArm;

    UPROPERTY(VisibleAnywhere) 
    UCameraComponent* viewCamera; 

    UPROPERTY(VisibleAnywhere) 
    USphereComponent* sphereComponent;
};
