#pragma once

#include "CoreMinimal.h"
#include "WGPItem.h"
#include "Soul.generated.h"

UCLASS()
class WGPOPENWORLD_API ASoul : public AWGPItem
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void OnBeginOverlap_WGP(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere)
	int32 Souls;
	double DesiredZ;

	UPROPERTY(EditAnywhere)
	float DriftRate = -15.f;

public:
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE void SetSouls(int32 Val) { Souls = Val; }
};