#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "Abilities/GameplayAbility.h"
#include "EquipWeapon.generated.h"

UCLASS()
class GASRPG_API UEquipWeapon : public UBaseGameplayAbility
{
	GENERATED_BODY()

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
