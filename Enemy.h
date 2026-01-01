#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "CharacterStates.h"
#include "Enemy.generated.h"

class UWidgetComponent;
class UHealthBarComponent;
class UPawnSensingComponent;

struct FAIRequestID;
struct FPathFollowingResult;

UCLASS()
class WGPOPENWORLD_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& impactPoint, AActor* Hitter) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;
	virtual void Die_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	virtual void Attack() override;

	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void AttackEnd() override;

	/** 当AI控制器占有该Pawn时回调 */
	virtual void PossessedBy(AController* NewController) override;

	//UFUNCTION()
	void OnMoveCompleted_WGP(FAIRequestID RequestID, const FPathFollowingResult& Result); // FPathFollowingResult在PathFollowingComponent.h中定义

private:
	void InitializeEnemy();
	void EquipDefaultWeapon();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();

	FTimerHandle PatrolTimer;

	/** AI behavior */
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsEngaged();
	bool IsDead();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();

	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();

	void ForceStopMovement();

	// 平滑距离判断，加入“滞回”避免边界抖动
	void UpdateRangeStates();

	void SpawnSoul();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	void DisplayDebugInfo();
	void DisplayDebugInfo2();

	UPROPERTY()
	class AAIController* EnemyController;

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* healthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWGPWeapon> WeaponClass;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double PatrolRadius =800.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double CombatRadius =600.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double AttackRadius =150.f;

	//进入/退出半径的滞回量，避免临界点抖动
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float CombatRadiusHysteresis =50.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRadiusHysteresis =30.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double AcceptanceRadius =10.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin =2.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax =4.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float PatrollingSpeed =125.f;

	/** combat */
	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin =0.5f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax =1.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChasingSpeed =300.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeathLifeSpan =8.f;

	EEnemyState LastEnemyState = EEnemyState::EES_NoState; // 上一帧 enemy 的状态值
	int LastPlayerPositionScope =3; // 上一帧 player 的位置相对于范围：0/1(攻击范围)、2(追逐范围)、3(安全范围)
	bool bIsEnemyMoving = false;

	// 平滑后的范围状态
	bool bInCombatRange = false;
	bool bInAttackRange = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASoul> SoulClass;
};