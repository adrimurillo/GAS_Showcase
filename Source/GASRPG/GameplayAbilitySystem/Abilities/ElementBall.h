#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "Abilities/GameplayAbility.h"
#include "ElementBall.generated.h"


class AGameplayAbilityWorldReticle;
enum class EGASAbilityInputID : uint8;
class AEffectAreaBase;

UCLASS()
class GASRPG_API UElementBall : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElementBall", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> ElementBallClass;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElementBall", meta = (AllowPrivateAccess = "true"))
    //TSubclassOf<UGameplayEffect> ElementBallImpactClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElementBall", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElementBall", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AEffectAreaBase> EffectAreaClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElementBall", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGameplayAbilityWorldReticle> ReticleClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElementBall", meta = (AllowPrivateAccess = "true"))
	float SphereRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElementBall", meta = (AllowPrivateAccess = "true"))
	float ImpactAreaDuration = 5.f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElementBall", meta = (AllowPrivateAccess = "true"))
	//FGameplayTag ImpactTag;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& Data);
	UFUNCTION()
	void OnTargetCancelled(const FGameplayAbilityTargetDataHandle& Data);
};
