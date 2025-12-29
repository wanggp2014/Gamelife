#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WGPItem.generated.h"

class UStaticMeshComponent;
class USphereComponent;

enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

UCLASS()
class WGPOPENWORLD_API AWGPItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWGPItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
		
	UFUNCTION()
	virtual void OnBeginOverlap_WGP(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap_WGP(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintPure)
	float Cos_With_Time();

	UFUNCTION(BlueprintPure)
	float Sin_With_Time();

	void DisplayDebugMessage(const FString& msg, float Duration, FColor Color);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SpawnPickupEffect();
	virtual void SpawnPickupSound();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* aMesh;

	EItemState itemState = EItemState::EIS_Hovering;

	UPROPERTY(EditAnywhere)
	USphereComponent* aSphereComponent;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* ItemEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;

private:	
	UPROPERTY(EditAnywhere)
	float RunningTime;

	UPROPERTY(EditAnywhere)
	float Amplitude;

	UPROPERTY(EditAnywhere)
	float TimeFrequency;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* PickupEffect;
};
