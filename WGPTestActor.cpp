#include "WGPTestActor.h"
#include "DebugMacros.h"

// Sets default values
AWGPTestActor::AWGPTestActor() : Amplitude(8.f), Period(20.f), Speed(2.f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize sm_cube
	sm_cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("A Cube By WGP"));
	this->RootComponent = sm_cube;
}

// Called when the game starts or when spawned
void AWGPTestActor::BeginPlay()
{
	Super::BeginPlay();

	//if (this->sm_cube)
	//{
	//	// Set the static mesh for sm_cube
	//	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/Content/BasicShapes/Cube.Cube"));
	//	if (CubeMeshAsset.Succeeded())
	//	{
	//		sm_cube->SetStaticMesh(CubeMeshAsset.Object); 
	//	}
	//	// Attach sm_cube to the root component
	//	sm_cube->SetupAttachment(RootComponent);

	//	FString retNote = TEXT("create cube asset! by WGP ===========");
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *retNote);
	//}

	FString MyString = TEXT("Hello, World! By WGP!");

	// Log the string to the output log
	UE_LOG(LogTemp, Warning, TEXT("%s"), *MyString);

	// Display a string on the game screen
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, MyString);

	FVector Location = GetActorLocation();
	// Calculate the end point of the debug line by adding [0, 0, 100] to the actor's location
	FVector EndPoint = Location + GetActorRightVector() * 200.f;

	//DRAW_SPHERE(Location); // Draw a debug sphere
	//DRAW_LINE(Location, EndPoint); // Draw a debug line
	//DRAW_POINT(EndPoint); // Draw a debug point
	//DRAW_VECTOR(Location, EndPoint);

	int result1 = Avg<int>(5, 10); // 使用 Sum 模板函数对两个整数求和
	float result2 = Avg<float>(3.14f, 2.71f); // 使用 Sum 模板函数对两个浮点数求和

	FVector vector1(1.f, 2.f, 3.f);
	FVector vector2(4.f, 5.f, 6.f);
	FVector result = Avg<FVector>(vector1, vector2);

	// Output the results to the UE5 log
	UE_LOG(LogTemp, Warning, TEXT("result1: %d"), result1);
	UE_LOG(LogTemp, Warning, TEXT("result2: %f"), result2);
	UE_LOG(LogTemp, Warning, TEXT("result: %s"), *result.ToString());

	//FRotator rotator1(0.f, 0.f, 0.f);
	//FRotator rotator2(90.f, 0.f, 0.f);
	//FRotator result3 = Avg<FRotator>(rotator1, rotator2);
}

// Called every frame
void AWGPTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//// Get the current time
	//FDateTime CurrentTime = FDateTime::Now();

	//// Convert the current time to a string
	//FString TimeString = CurrentTime.ToString();

	//// Log the current time to the output log
	//UE_LOG(LogTemp, Warning, TEXT("Current Time: %s"), *TimeString);

	//FVector Location = GetActorLocation();
	//FVector EndPoint = Location + FVector(0, 3.f, 1.f);		
	//SetActorLocation(EndPoint);

 //   FRotator Rotation = GetActorRotation();
 //   Rotation.Yaw += 5.0f; // Rotate by 1 degree around Z axis
 //   SetActorRotation(Rotation);

	//DRAW_SPHERE_SingleFrame(EndPoint);

	//FVector LineEndPoint = EndPoint + GetActorForwardVector() * 200.f;
	//DRAW_LINE_SingleFrame(EndPoint, LineEndPoint);

	//float MovementRate = 50.f;
	//AddActorWorldOffset(FVector(0, MovementRate * DeltaTime, 0));
	//DRAW_SPHERE_SingleFrame(GetActorLocation());

	// Get the location of this actor
	FVector ActorLocation = GetActorLocation();
	// Calculate the end point of the debug line by adding [0, 0, 100] to the actor's location
	FVector EndPoint = ActorLocation + GetActorForwardVector() * 200.f;

	// Modify the z component of the actor's location using a sine wave
	float SinValue = FMath::Sin(this->Period * GetGameTimeSinceCreation());
	//ActorLocation.Y += this->Speed;
	ActorLocation.Z += SinValue * this->Amplitude;
	SetActorLocation(ActorLocation); // Set the new location of the actor

	// Draw a debug line from the actor's location to the calculated end point with red color for 5 seconds
	//DrawDebugLine(GetWorld(), ActorLocation, EndPoint, FColor::Red, false, 30.0f, 0, 5.0f);
	DRAW_SPHERE_SingleFrame(ActorLocation);
	DRAW_LINE_SingleFrame(ActorLocation, EndPoint);

	this->WgpEvent(1.0f, TEXT("Hello, World!"));
}

FString AWGPTestActor::WgpFunc(float InFloat, const FString& InString)
{
	FString Result = FString::Printf(TEXT("InFloat: %f, InString: %s"), InFloat, *InString);
	return Result;
}

FString AWGPTestActor::WgpFunc_Pure(float InFloat, const FString& InString) const
{
	FString Result = FString::Printf(TEXT("InFloat: %f, InString: %s"), InFloat, *InString);
	return Result;
}

//void AWGPTestActor::WgpEvent_Implementation(float InFloat, const FString& InString)
//{
//	FString Result = FString::Printf(TEXT("InFloat: %f, InString: %s"), InFloat, *InString);
//
//	// Display a string on the game screen
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Result);
//}

// Add the implementation of the template function
template<typename T>
inline T AWGPTestActor::Avg(T a, T b)
{
	return (a + b)/2;
}
