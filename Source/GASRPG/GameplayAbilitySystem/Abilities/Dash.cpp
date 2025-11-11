#include "Dash.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/RootMotionSource.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GASRPG/GASRPG.h"
#include "GASRPG/Util/LoggerUtil.h"


void UDash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                            const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CommitAbility(Handle, ActorInfo, ActivationInfo);

	Pawn = Cast<APawn>(ActorInfo->AvatarActor.Get());
	if (!IsValid(Pawn))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
	}

	//FVector DashDirection;
	//if (TriggerEventData && TriggerEventData->TargetData.Num() > 0)
	//{
	//	const FGameplayAbilityTargetData_Dash* DashData = static_cast<const FGameplayAbilityTargetData_Dash*>(TriggerEventData->TargetData.Get(0));
//
	//	if (DashData)
	//	{
	//		DashDirection = DashData->DashDirection;
	//	}
	//}
	FVector DashDirection = Pawn->GetActorForwardVector();
	
	UAbilityTask_ApplyRootMotionConstantForce* DashTask =
		UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
			this,
			NAME_None,
			DashDirection,
			DashStrength,
			DashDuration,
			false,
			nullptr,
			ERootMotionFinishVelocityMode::ClampVelocity,
			FVector::ZeroVector,
			GetMaxSpeed(),
			false
			);

	if (IsValid(DashTask))
	{
		DashTask->OnFinish.AddDynamic(this, &UDash::OnDashFinished);
		DashTask->ReadyForActivation();
		
		GetAbilitySystemComponentFromActorInfo_Ensured()->AddGameplayCue(DashCueTag);
	}
}

void UDash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	GetAbilitySystemComponentFromActorInfo_Ensured()->RemoveGameplayCue(DashCueTag);
	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false);
}

void UDash::OnDashFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

FVector UDash::GetDashDirection() const
{
	if (!IsValid(Pawn))
	{
		return Pawn->GetActorForwardVector();
	}

	FVector Velocity = Pawn->GetVelocity().GetSafeNormal();
	Velocity.Z = 0.0f;
	return Velocity == FVector::ZeroVector ? Pawn->GetActorForwardVector() : Velocity;	
}

float UDash::GetMaxSpeed() const
{
	if (!IsValid(Pawn) || !IsValid(Pawn->GetMovementComponent()))
	{
		return DefaultMaxSpeed;
	}

	return Pawn->GetMovementComponent()->GetMaxSpeed();
}
