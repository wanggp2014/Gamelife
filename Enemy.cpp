#include "Enemy.h"
#include "Components/SkeletalMeshComponent.h" 
#include "DebugMacros.h"
#include "Animation/AnimMontage.h"
#include "Components/WidgetComponent.h"
#include "Components/AttributeComponent.h"
#include "Weapon/WGPWeapon.h"
#include "HUD/HealthBarComponent.h"
#include <Perception/PawnSensingComponent.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "Soul.h"
#include "Navigation/PathFollowingComponent.h" 

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	this->GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic); // 原来是 ECC_WorldDynamic；ECC_Pawn
	this->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	this->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	this->GetMesh()->SetGenerateOverlapEvents(true);

	this->healthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarWidget"));
	this->healthBarWidget->SetupAttachment(this->GetRootComponent());

	this->GetCharacterMovement()->bOrientRotationToMovement = true;
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationYaw = false;
	this->bUseControllerRotationRoll = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
}

void AEnemy::HideHealthBar()
{
	if (this->healthBarWidget)
		this->healthBarWidget->SetVisibility(false);
}

void AEnemy::ShowHealthBar()
{
	if (this->healthBarWidget)
		this->healthBarWidget->SetVisibility(true);
}

void AEnemy::LoseInterest()
{
	this->CombatTarget = nullptr;
	HideHealthBar();
	// 重置平滑范围，避免残留状态
	bInCombatRange = false;
	bInAttackRange = false;
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatrolTarget);
}

void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;

	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemy::DisplayDebugInfo()
{
#if WITH_EDITOR
	if (PawnSensing)
	{
		// 显示视野角度
		const FVector Forward = GetActorForwardVector();
		const float Angle = PawnSensing->GetPeripheralVisionAngle();
		const float Radius = PawnSensing->SightRadius;
		const int Segments =32;
		const float Step = Angle *2 / Segments;
		for (int i =0; i < Segments; ++i)
		{
			const float CurrentAngle = -Angle + i * Step;
			const float NextAngle = -Angle + (i +1) * Step;
			const FVector StartDir = Forward.RotateAngleAxis(CurrentAngle, FVector::UpVector);
			const FVector EndDir = Forward.RotateAngleAxis(NextAngle, FVector::UpVector);
			DrawDebugLine(
				GetWorld(),
				GetActorLocation(),
				GetActorLocation() + StartDir * Radius,
				FColor::Yellow,
				false,
				-1.f,
				0,
				1.f
			);
			DrawDebugLine(
				GetWorld(),
				GetActorLocation() + StartDir * Radius,
				GetActorLocation() + EndDir * Radius,
				FColor::Yellow,
				false,
				-1.f,
				0,
				1.f
			);
		}
	}
#endif

#if WITH_EDITOR
	DrawDebugSphere(GetWorld(), GetActorLocation(), CombatRadius,15, FColor::Red, false,0,0,1.f);
	DrawDebugSphere(GetWorld(), GetActorLocation(), AttackRadius,15, FColor::Red, false,0,0,1.f);
#endif

#if WITH_EDITOR
	// 显示敌人当前状态文本
	FVector TextLocation = GetActorLocation() + FVector(0,0,120.f);
	FColor StateColor = FColor::Red;
	FString StateText;
	switch (EnemyState)
	{
	case EEnemyState::EES_Patrolling:
		StateText = TEXT("Patrolling");
		StateColor = FColor::Green;
		break;
	case EEnemyState::EES_Chasing:
		StateText = TEXT("Chasing");
		StateColor = FColor::Yellow;
		break;
	case EEnemyState::EES_Attacking:
		StateText = TEXT("Attacking");
		StateColor = FColor::Red;
		break;
	case EEnemyState::EES_Engaged:
		StateText = TEXT("Engaged");
		StateColor = FColor::Orange;
		break;
	case EEnemyState::EES_Dead:
		StateText = TEXT("Dead");
		StateColor = FColor::Black;
		break;
	default:
		StateText = TEXT("Idle");
		StateColor = FColor::White;
		break;
	}
	DrawDebugString(GetWorld(), TextLocation, StateText, nullptr, StateColor,0.f, true);
#endif
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);

	InitializeEnemy();
}

void AEnemy::InitializeEnemy()
{
	healthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	healthBarWidget->SetHealthPercent(1.f);
	PawnSensing->SightRadius = PatrolRadius;
	PawnSensing->SetPeripheralVisionAngle(45.f);

	if (!EnemyController) EnemyController = Cast<AAIController>(GetController());

	MoveToTarget(PatrolTarget);
	HideHealthBar();
	EquipDefaultWeapon();

	Tags.Add(FName("Enemy"));
}

void AEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!EnemyController) EnemyController = Cast<AAIController>(GetController());
	if (EnemyController)
	{
		// 获取路径跟随组件
		UPathFollowingComponent* PathFollowingComp = EnemyController->GetPathFollowingComponent();
		if (PathFollowingComp)
		{
			// FAIRequestDelegate是一个多播委托，使用AddUObject来绑定
			PathFollowingComp->OnRequestFinished.AddUObject(this, &AEnemy::OnMoveCompleted_WGP);
		}
	}
}

void AEnemy::EquipDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWGPWeapon* DefaultWeapon = World->SpawnActor<AWGPWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
		this->equipWeapon = DefaultWeapon;
	}
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void AEnemy::Die_Implementation()
{
	Super::Die_Implementation();

	ClearAttackTimer();
	ClearPatrolTimer();

	EnemyState = EEnemyState::EES_Dead;

	PlayDeathMontage();

	HideHealthBar();
	DisableCapsule();
	SetLifeSpan(DeathLifeSpan);

	//GetCharacterMovement()->bOrientRotationToMovement = false;
	//enableWeaponCollision(); //解决这个问题：enemy 已经死了，但身上的宝剑跌落过程中仍会伤者 player

	SpawnSoul();
}

void AEnemy::SpawnSoul()
{
	UWorld* World = GetWorld();
	if (World && SoulClass && attributes)
	{
		const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 125.f);
		ASoul* SpawnedSoul = World->SpawnActor<ASoul>(SoulClass, SpawnLocation, GetActorRotation());
		if (SpawnedSoul)
		{
			SpawnedSoul->SetSouls(attributes->GetSouls()); // enemy 自己的 attributes
			SpawnedSoul->SetOwner(this);
		}
	}
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;

	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);

	bIsEnemyMoving = true;
	EnemyController->MoveTo(MoveRequest);

	//DisplayDebugMessage(TEXT("MoveToTarget..."),5.f, FColor::Blue);
}

void AEnemy::OnMoveCompleted_WGP(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (bIsEnemyMoving) bIsEnemyMoving = false;

	switch (Result.Code)
	{
	case EPathFollowingResult::Success:
		//DisplayDebugMessage(TEXT("Move successful!"),5.f, FColor::Blue);
		break;

	case EPathFollowingResult::Blocked:
		//DisplayDebugMessage(TEXT("Move blocked!"),5.f, FColor::Red);
		break;

	case EPathFollowingResult::OffPath: // 偏离了导航网格
		//DisplayDebugMessage(TEXT("Fell off the path."),5.f, FColor::Red);
		break;

	case EPathFollowingResult::Aborted: // 移动被另一个移动请求或逻辑中断
		//DisplayDebugMessage(TEXT("Move aborted!"),5.f, FColor::Red);
		break;

	case EPathFollowingResult::Invalid:
	default:
		//DisplayDebugMessage(TEXT("Move failed: Invalid path or goal."),5.f, FColor::Red);
		break;
	}
}

AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target:PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets >0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets -1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget = 
		EnemyState != EEnemyState::EES_Dead && 
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState != EEnemyState::EES_Attacking &&
		EnemyState != EEnemyState::EES_Engaged &&
		SeenPawn->ActorHasTag(FName("EngageableTarget"));

	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();
	}
}

void AEnemy::Attack()
{
	Super::Attack();

	if (CombatTarget == nullptr)
	{
		StartPatrolling();
		return;
	}

	EnemyState = EEnemyState::EES_Engaged;

	PlayAttackMontage();
	AfterAttackTriggered();
}

bool AEnemy::CanAttack()
{
	bool bCanAttack = 
		bInAttackRange && 
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();

	return bCanAttack;
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);

	if (this->attributes && this->healthBarWidget)
	{
		this->healthBarWidget->SetHealthPercent(attributes->getHealthPercent());
	}
}

void AEnemy::AttackEnd()
{
	if (!IsDead()) EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DisplayDebugInfo();
	//DisplayDebugInfo2();

	if (IsDead()) return;

	//先更新平滑的距离范围状态
	UpdateRangeStates();

	if (EnemyState > EEnemyState::EES_Patrolling)
		CheckCombatTarget();
	else 
		CheckPatrolTarget();
}

void AEnemy::DisplayDebugInfo2()
{
	int ThisPlayerPositionScope;
	if (IsOutsideCombatRadius()) ThisPlayerPositionScope =3; // 安全范围
	else if (IsOutsideAttackRadius()) ThisPlayerPositionScope =2; //追逐范围
	else if (CanAttack()) ThisPlayerPositionScope =1; // 攻击范围且可攻击
	else ThisPlayerPositionScope =0; // 攻击范围但不可攻击

	if (
		ThisPlayerPositionScope != LastPlayerPositionScope ||
		EnemyState != LastEnemyState
		)
	{
		DisplayDebugMessage(TEXT("============================"),10.f, FColor::Green);

		FString Msg = FString::Printf(TEXT("%d"), ThisPlayerPositionScope);
		DisplayDebugMessage(Msg,10.f, FColor::Green);

		DisplayDebugMessage(EnemyState,10.f, FColor::Green);

		if (bIsEnemyMoving) 
			DisplayDebugMessage(TEXT("Enemy is moving..."),10.f, FColor::Green);
		else
			DisplayDebugMessage(TEXT("Enemy stop!"),10.f, FColor::Green);

		if (CombatTarget)
			DisplayDebugMessage(TEXT("Player checked..."),10.f, FColor::Green);
		else
			DisplayDebugMessage(TEXT("No player!"),10.f, FColor::Green);

		LastEnemyState = EnemyState;
		LastPlayerPositionScope = ThisPlayerPositionScope;
	}
}

void AEnemy::CheckPatrolTarget()
{
	if (IsDead()) return;

	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();

		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
	}
}

void AEnemy::CheckCombatTarget()
{
	if (IsDead()) return;

	if (CombatTarget)
	{
		// 使用带滞回的范围状态避免边界抖动
		if (!bInCombatRange)
		{
			ClearAttackTimer();
			LoseInterest();
			if (!IsEngaged()) StartPatrolling();
		}
		else if (!bInAttackRange)
		{
			if (!IsChasing())
			{
				ClearAttackTimer();
				if (!IsEngaged()) ChaseTarget();
			}
		}
		else
		{
			// 如果导航运动还在进行中，则强制停止并准备攻击
			ForceStopMovement();

			if (CanAttack())
			{
				StartAttackTimer();
			}
		}
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	
	if (!IsDead()) ShowHealthBar();

	//ClearPatrolTimer();
	//ClearAttackTimer();
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	ChaseTarget();

	return 0.0f;
}

void AEnemy::Destroyed()
{
	if (equipWeapon)
	{
		equipWeapon->Destroy();
	}
}

void AEnemy::ForceStopMovement()
{
	if (EnemyController && bIsEnemyMoving) EnemyController->StopMovement();
}

void AEnemy::UpdateRangeStates()
{
	if (!CombatTarget)
	{
		bInCombatRange = false;
		bInAttackRange = false;
		return;
	}

	const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();
	//FString Msg = FString::Printf(TEXT("%f"), DistanceToTarget);
	//DisplayDebugMessage(Msg, 10.f, FColor::Green);

	// CombatRange 滞回：进入阈值 = CombatRadius；退出阈值 = CombatRadius + CombatRadiusHysteresis
	if (bInCombatRange)
	{
		if (DistanceToTarget > CombatRadius + CombatRadiusHysteresis)
		{
			bInCombatRange = false;
		}
	}
	else
	{
		if (DistanceToTarget < CombatRadius)
		{
			bInCombatRange = true;
		}
	}

	// AttackRange 滞回：进入阈值 = AttackRadius；退出阈值 = AttackRadius + AttackRadiusHysteresis
	if (bInAttackRange)
	{
		//Msg = FString::Printf(TEXT("%f"), AttackRadius + AttackRadiusHysteresis);
		//DisplayDebugMessage(Msg, 10.f, FColor::Red);

		if (DistanceToTarget > AttackRadius + AttackRadiusHysteresis)
		{
			bInAttackRange = false;
		}
	}
	else
	{
		if (DistanceToTarget < AttackRadius)
		{
			bInAttackRange = true;
		}
	}
}