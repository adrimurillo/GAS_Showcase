#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "Silence.generated.h"


UCLASS()
class GASRPG_API USilence : public UBaseGameplayAbility
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Silence", meta = (AllowPrivateAccess = true))
	float SilenceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Silence", meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> SilenceEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Silence", meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> OwnSilenceEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Silence", meta = (AllowPrivateAccess = true))
	float SilenceDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Silence", meta = (AllowPrivateAccess = true))
	FGameplayTag SilenceDurationTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Silence", meta = (AllowPrivateAccess = true))
	float TimeBetweenActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Silence", meta = (AllowPrivateAccess = true))
	int32 TotalActionCount;

private:
	USilence();
		
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void PerformSilence(int32 ActionNumber);

	UFUNCTION()
	void OnFinished(int32 ActionNumber);
};

