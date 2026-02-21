# CLAUDE.md

Guidelines for AI assistants working on this repository.

## Project Overview

This is an **Unreal Engine 5 C++ game module** (`WGPOPENWORLD_API`) for an open-world action game. It implements enemy AI, combat mechanics, item pickups, and a soul/currency system. The source files in this repository are part of a larger UE5 project — no `.uproject` or build files are included here.

## Repository Structure

```
Gamelife/
├── CharacterStates.h      # Enums: ECharacterState, EActionState, EDeathPose, EEnemyState
├── DebugMacros.h           # Debug drawing macros (spheres, lines, points, vectors)
├── Enemy.h / Enemy.cpp     # Enemy AI actor (state machine, patrol, chase, attack)
├── Soul.h / Soul.cpp       # Soul collectible dropped on enemy death
├── WGPItem.h / WGPItem.cpp # Base item class with hover animation and pickup
├── WGPTestActor.h / .cpp   # Development/debug test actor
├── WgpPawn.h / WgpPawn.cpp # Player-controlled pawn with camera rig
└── README.md
```

All source files are at the repository root — there are no subdirectories.

## Key Systems

### Enemy AI (`Enemy.h/cpp`)
State-driven AI using `EEnemyState`:
- **Patrolling** — moves between `PatrolTargets` set in the editor
- **Chasing** — pursues the player when detected via `UPawnSensingComponent`
- **Attacking** / **Engaged** — melee combat within `AttackRadius`
- **Dead** — plays death animation, spawns Soul, self-destructs after `DeathLifeSpan`

Range transitions use hysteresis (`CombatRadiusHysteresis`, `AttackRadiusHysteresis`) to prevent state flickering at boundaries.

### Item System (`WGPItem.h/cpp`, `Soul.h/cpp`)
- `AWGPItem` — base pickupable actor with sine-wave hover, Niagara VFX, and sound on pickup
- `ASoul` — subclass dropped by enemies on death; drifts downward and is collectible by the player

### Player Pawn (`WgpPawn.h/cpp`)
Spring-arm camera setup with basic movement input (Move, Turn, Lookup).

## Architecture & Dependencies

Classes extend UE5 base classes from an external `BaseCharacter` hierarchy:
- `AEnemy` extends `ABaseCharacter` (not in this repo)
- `AWGPItem` extends `AActor`
- `AWgpPawn` extends `APawn`

External dependencies referenced but not included in this repo:
- `ABaseCharacter`, `UAttributeComponent`, `UHealthBarComponent`
- `AWGPWeapon`, `PickupInterface`

## Code Conventions

### Naming
- **Classes**: UE5 prefix convention — `A` for Actors (`AEnemy`), `U` for UObjects, `E` for enums (`EEnemyState`)
- **Enum values**: Prefixed with abbreviation — `EES_Patrolling`, `EDP_Death_1`, `EAS_Attacking`
- **Functions**: `PascalCase` — `CanAttack()`, `StartPatrolling()`, `MoveToTarget()`
- **Member variables**: `camelCase` — `healthBarWidget`, `bInCombatRange`
- **Bool prefix**: `b` — `bInCombatRange`, `bIsEnemyMoving`

### Headers
- Use `#pragma once` (no include guards)
- Forward-declare classes when possible instead of including headers
- UE5 generated headers included last (`*.generated.h`)

### UE5 Macros
- All reflected classes use `UCLASS()` / `GENERATED_BODY()`
- Properties exposed to the editor use `UPROPERTY(EditAnywhere)` or `UPROPERTY(VisibleAnywhere)`
- Blueprint-callable functions use `UFUNCTION()`
- Categories group related properties: `"AI Navigation"`, `"Combat"`

### Comments
- Mix of English and Chinese comments throughout the codebase
- English used for section headers and API docs
- Chinese used for implementation notes and explanations

### Debug Code
- Debug visualization guarded by `#if WITH_EDITOR`
- Use macros from `DebugMacros.h` (`DRAW_SPHERE`, `DRAW_LINE`, etc.)
- `_SingleFrame` variants for per-frame debug drawing

## Build & Testing

- **Build system**: This module is compiled as part of a larger UE5 project via Unreal Build Tool. There are no standalone build files in this repo.
- **Tests**: No automated tests exist. `WGPTestActor` is a runtime debug actor, not a test suite.
- **CI/CD**: None configured.

## Development Notes

- The module exports its API via `WGPOPENWORLD_API` macro
- To build, these files must be placed within a UE5 project's `Source/` directory alongside the missing dependencies (`BaseCharacter`, `WGPWeapon`, etc.)
- Default property values are set inline in headers (e.g., `CombatRadius = 600.f`)
- Timer-based mechanics use `GetWorldTimerManager()` for patrol waits and attack cooldowns
