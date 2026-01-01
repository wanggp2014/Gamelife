#include "WGPItem.h"
#include <Components/SphereComponent.h>
//#include "Characters/WGPCharacter.h"
#include "Interfaces/PickupInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AWGPItem::AWGPItem()
{
	PrimaryActorTick.bCanEverTick = true;

    aMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("aMesh"));
    aMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    aMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RootComponent = aMesh;

    aSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("aSphereComponent"));
	aSphereComponent->SetupAttachment(GetRootComponent());

    this->ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
    this->ItemEffect->SetupAttachment(GetRootComponent());
}

void AWGPItem::BeginPlay()
{
	Super::BeginPlay();

    aSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AWGPItem::OnBeginOverlap_WGP);
    aSphereComponent->OnComponentEndOverlap.AddDynamic(this, &AWGPItem::OnEndOverlap_WGP);
}

void AWGPItem::SpawnPickupEffect()
{
    if (PickupEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            this,
            PickupEffect,
            GetActorLocation()
        );
    }
}

void AWGPItem::SpawnPickupSound()
{
    if (PickupSound)
    {
        UGameplayStatics::SpawnSoundAtLocation(
            this,
            PickupSound,
            GetActorLocation()
        );
    }
}

void AWGPItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    this->RunningTime += DeltaTime;

    if (itemState == EItemState::EIS_Hovering)
    {
        AddActorWorldOffset(FVector(0.f, 0.f, Sin_With_Time()));
    }
}

void AWGPItem::OnBeginOverlap_WGP(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    IPickupInterface* HitInterface = Cast<IPickupInterface>(OtherActor);
    if (HitInterface) HitInterface->SetOverlappingItem(this);
}

void AWGPItem::OnEndOverlap_WGP(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    IPickupInterface* HitInterface = Cast<IPickupInterface>(OtherActor);
    if (HitInterface) HitInterface->SetOverlappingItem(nullptr);
}

float AWGPItem::Cos_With_Time()
{
    return Amplitude * FMath::Cos(TimeFrequency * RunningTime);
}

float AWGPItem::Sin_With_Time()
{
    return Amplitude * FMath::Sin(TimeFrequency * RunningTime);
}

void AWGPItem::DisplayDebugMessage(const FString& msg, float Duration, FColor Color)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,               // 键值（-1表示自动分配）
            Duration,         // 显示时间
            Color,            // 文字颜色
            msg           // 显示内容
        );
    }
}