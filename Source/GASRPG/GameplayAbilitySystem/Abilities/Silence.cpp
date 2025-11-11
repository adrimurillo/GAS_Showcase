#include "Silence.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GASRPG/GASRPG.h"
#include "Abilities/Tasks/AbilityTask_Repeat.h"
#include "GASRPG/GameplayAbilitySystem/Characters/APlayerCharacter.h"
#include "GASRPG/Util/LoggerUtil.h"
#include "Kismet/KismetSystemLibrary.h"

USilence::USilence()
{
	SilenceRadius = 500.0f;
	SilenceDuration = 1.f;
	TimeBetweenActions = 1.f;
	TotalActionCount = 4;
}

void USilence::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbilityCost(Handle, ActorInfo, ActivationInfo);
	//CommitAbility(Handle, ActorInfo, ActivationInfo);
	UAbilityTask_Repeat* RepeatTask = UAbilityTask_Repeat::RepeatAction(this, TimeBetweenActions, TotalActionCount);
	RepeatTask->OnPerformAction.AddDynamic(this, &USilence::PerformSilence);
	RepeatTask->OnFinished.AddDynamic(this, &USilence::OnFinished);
	RepeatTask->ReadyForActivation();
	
}

void USilence::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USilence::PerformSilence(int32 ActionNumber)
{
	TArray<AActor*> HitActors;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Ensured();
	if (IsValid(ASC))
	{
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(OwnSilenceEffect, 1, ASC->MakeEffectContext());
		EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(SilenceDurationTag, SilenceDuration);
		ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}

	AActor* Avatar = CurrentActorInfo->AvatarActor.Get();
	if (!IsValid(Avatar))
	{
		return;
	}
	UKismetSystemLibrary::SphereOverlapActors(this, Avatar->GetActorLocation(), SilenceRadius, { ObjectTypeQuery3 }, AAPlayerCharacter::StaticClass(), {Avatar}, HitActors);
	DrawDebugSphere(GetWorld(), Avatar->GetActorLocation(), SilenceRadius, 16, FColor::Red);

	for (AActor* HitActor : HitActors)
	{
		UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (IsValid(AbilitySystemComponent) && IsValid(SilenceEffect))
		{
			FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(SilenceEffect, 1, AbilitySystemComponent->MakeEffectContext());
			EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(SilenceDurationTag, SilenceDuration);
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}

}

void USilence::OnFinished(int32 ActionNumber)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
