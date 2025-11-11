#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "Abilities/GameplayAbility.h"
#include "Dash.generated.h"


enum class EGASAbilityInputID : uint8;

USTRUCT()
struct FGameplayAbilityTargetData_Dash : public FGameplayAbilityTargetData
{
	GENERATED_BODY()

	FVector DashDirection = FVector::ZeroVector;
};

UCLASS()
class GASRPG_API UDash : public UBaseGameplayAbility
{
	GENERATED_BODY()

	UPROPERTY()
	APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float DefaultMaxSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float DashStrength = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float DashDuration = 0.3f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	FGameplayTag DashCueTag;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void OnDashFinished();

	UFUNCTION(BlueprintCallable, Category = "Dash")
	FVector GetDashDirection() const;
	float GetMaxSpeed() const;
};
