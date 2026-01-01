// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WGPTestActor.generated.h"

UCLASS()
class WGPOPENWORLD_API AWGPTestActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWGPTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION(BlueprintPure, Category = "WGPTest")
    FString WgpFunc_Pure(float InFloat, const FString& InString) const;

    UFUNCTION(BlueprintCallable, Category = "WGPTest")
    FString WgpFunc(float InFloat, const FString& InString);

    UFUNCTION(BlueprintImplementableEvent, Category = "WGPTest")
    void WgpEvent(float InFloat, const FString& InString);

    //UFUNCTION(BlueprintNativeEvent, Category = "WGPTest")
    //void WgpNativeEvent(float InFloat, const FString& InString);

    template<typename T>
	T Avg(T a, T b);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Expose properties to Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WGPTest")
	float Amplitude = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WGPTest")
	float Period = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WGPTest")
	float Speed = 2.f;

	// 以下两个仅用来测试

	UPROPERTY(EditDefaultsOnly, Category = "WGPTest")
	float myProp_EditDefaultsOnly = 10.f;

	UPROPERTY(EditInstanceOnly, Category = "WGPTest")
	float myProp_EditInstanceOnly = 5.f;

    // Expose properties to Blueprint
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WGPTest")
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WGPTest")
    UStaticMeshComponent* sm_cube;
};