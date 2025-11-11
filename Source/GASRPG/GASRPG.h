#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGASAbilityInputID : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),
	// 1 Confirm
	Confirm			UMETA(DisplayName = "Confirm"),
	// 2 Cancel
	Cancel			UMETA(DisplayName = "Cancel"),
	// 3 Q
	Ability1		UMETA(DisplayName = "Ability1"),
	// 4 E
	Ability2		UMETA(DisplayName = "Ability2"),
	// 5 C
	Ability3		UMETA(DisplayName = "Ability3"),
	// 6 x
	Ability4		UMETA(DisplayName = "Ability4"),
	// 7 1
	Weapon1			UMETA(DisplayName = "Weapon1"),
	// 8 2
	Weapon2			UMETA(DisplayName = "Weapon2"),
	// 9 3
	Weapon3			UMETA(DisplayName = "Weapon3"),
};
