#include "ElementBall.h"

#include "GASRPG/Util/LoggerUtil.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Abilities/GameplayAbilityTargetActor_SingleLineTrace.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AGameplayAbilityTargetActor_LinetraceCamera.h"
#include "GASRPG/GASRPG.h"
#include "GASRPG/GameplayAbilitySystem/EffectActors/EffectAreaBase.h"


void UElementBall::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	TSubclassOf<AGameplayAbilityTargetActor> TargetClass = AAGameplayAbilityTargetActor_LinetraceCamera::StaticClass();
	FTransform SpawnTransform = FTransform::Identity;

	// Spawn deferred
	AGameplayAbilityTargetActor* SpawnedActor = World->SpawnActorDeferred<AGameplayAbilityTargetActor>(
		TargetClass,
		SpawnTransform,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);


	if (!IsValid(SpawnedActor))
	{
		return;
	}

	//SpawnedActor->bDebug = true;
	SpawnedActor->StartLocation = FGameplayAbilityTargetingLocationInfo();
	SpawnedActor->StartLocation.LocationType = EGameplayAbilityTargetingLocationType::ActorTransform;
	SpawnedActor->StartLocation.SourceActor = GetAvatarActorFromActorInfo();
	SpawnedActor->ReticleClass = ReticleClass;
	SpawnedActor->ReticleParams = FWorldReticleParameters();

	UGameplayStatics::FinishSpawningActor(SpawnedActor, SpawnTransform);

	UAbilityTask_WaitTargetData* WaitTargetData = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(
		this,
		FName("ElementBall"),
		EGameplayTargetingConfirmation::UserConfirmed,
		SpawnedActor
	);
	
	WaitTargetData->ValidData.AddDynamic(this, &UElementBall::OnTargetDataReady);
	WaitTargetData->Cancelled.AddDynamic(this, &UElementBall::OnTargetCancelled);
	WaitTargetData->ReadyForActivation();
}

void UElementBall::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UElementBall::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& Data)
{
	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);

	FVector SpherePos = Data.Get(0)->GetHitResult()->bBlockingHit ? Data.Get(0)->GetEndPoint() : static_cast<FVector>(Data.Get(0)->GetHitResult()->TraceEnd);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjTypes;
	ObjTypes.Add(ObjectTypeQuery3);
	TArray<AActor*> ActorsToIgnore = {};
	TArray<AActor*> OutActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), SpherePos, SphereRadius, ObjTypes, nullptr, ActorsToIgnore, OutActors);
	//DrawDebugSphere(GetWorld(), SpherePos, SphereRadius, 12, FColor::Red, false, 5);
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpherePos);
	SpawnTransform.SetRotation(GetAvatarActorFromActorInfo()->GetActorQuat());

	AEffectAreaBase* SpawnedActor = GetWorld()->SpawnActorDeferred<AEffectAreaBase>(
		EffectAreaClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (IsValid(SpawnedActor))
	{
		SpawnedActor->SetLifeSpan(ImpactAreaDuration);
		SpawnedActor->SetAreaRadius(SphereRadius);
		UGameplayStatics::FinishSpawningActor(SpawnedActor, SpawnTransform);
	}


	TArray<TWeakObjectPtr<AActor>> Actors = Data.Get(0)->GetActors();
	for (AActor* HitActor : OutActors)
	{
		// Apply Damage Effect
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (!TargetASC || !DamageEffectClass)
		{
			continue;
		}

		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
		if (SpecHandle.IsValid())
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}

		FGameplayEffectSpecHandle SpecHandleBurn = MakeOutgoingGameplayEffectSpec(ElementBallClass, 1.f);
		if (SpecHandleBurn.IsValid())
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandleBurn.Data.Get());
		}
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UElementBall::OnTargetCancelled(const FGameplayAbilityTargetDataHandle& Data)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
