#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "Abilities/GameplayAbility.h"
#include "Flash.generated.h"

class ABaseProjectile;

UCLASS()
class GASRPG_API UFlash : public UBaseGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flash", meta = (AllowPrivateAccess = true))
	TSubclassOf<ABaseProjectile> FlashClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flash", meta = (AllowPrivateAccess = true))
	ABaseProjectile* SpawnedFlash;

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	
	UFUNCTION()
	void OnTargetDataReady_Internal(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	void OnTargetCancelled_Internal(const FGameplayAbilityTargetDataHandle& Data);

	virtual void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& Data);
	virtual void OnTargetCancelled(const FGameplayAbilityTargetDataHandle& Data);
};
