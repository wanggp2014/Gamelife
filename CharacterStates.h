#pragma once

UENUM(BlueprintType)
enum class ECharacterState :uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType)
enum class EActionState :uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_HitReaction UMETA(DisplayName = "HitReaction"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon"),
	EAS_Dodge UMETA(DisplayName = "Dodge"),
	EAS_Dead UMETA(DisplayName = "Dead")
};

/**
 在 C++ 的 enum class 中，如果没有显式指定枚举成员的值，第一个成员默认值为 0，后续成员依次递增。因此：
 enum class EDeathPose : uint8
{
    EDP_Alive,      // 0
    EDP_Death_1,    // 1
    EDP_Death_2,    // 2
    EDP_Death_3,    // 3
    EDP_Death_4,    // 4
    EDP_Death_5,    // 5
    EDP_Death_6     // 6
};
 */
UENUM(BlueprintType)
enum class EDeathPose :uint8
{
	EDP_Death_1 UMETA(DisplayName = "Death_1"),
	EDP_Death_2 UMETA(DisplayName = "Death_2"),
	EDP_Death_3 UMETA(DisplayName = "Death_3"),
	EDP_Death_4 UMETA(DisplayName = "Death_4"),
	EDP_Death_5 UMETA(DisplayName = "Death_5"),
	EDP_Death_6 UMETA(DisplayName = "Death_6"),

	EDP_MAX UMETA(DisplayName = "Death_MAX")
};

UENUM(BlueprintType)
enum class EEnemyState :uint8
{
	EES_NoState UMETA(DisplayName = "NoState"),
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged")
	//EES_NoState UMETA(DisplayName = "NoState")
};