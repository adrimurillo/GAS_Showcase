#include "Flash.h"

#include "AGameplayAbilityTargetActor_LinetraceCamera.h"
#include "Abilities/GameplayAbilityTargetActor_SingleLineTrace.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "GameFramework/Character.h"
#include "GASRPG/GASRPG.h"
#include "GASRPG/GameplayAbilitySystem/Spawneables/BaseProjectile.h"
#include "GASRPG/Util/LoggerUtil.h"
#include "Kismet/GameplayStatics.h"

void UFlash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(Character) || !IsValid(FlashClass))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	TSubclassOf<AGameplayAbilityTargetActor> TargetClass = AAGameplayAbilityTargetActor_LinetraceCamera::StaticClass();
	FTransform SpawnTransform = FTransform::Identity;

	// Spawn deferred
	AGameplayAbilityTargetActor* SpawnedActor = World->SpawnActorDeferred<AGameplayAbilityTargetActor>(
		TargetClass,
		SpawnTransform,
		Character,
		Cast<APawn>(Character),
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
	
	// Spawn Flash
	if (HasAuthority(&CurrentActivationInfo))
	{
		UMeshComponent* CharacterMesh = Character->GetMesh();
		FVector SpawnLoc = Character->GetMesh()->GetSocketLocation(FName("FlashSocket"));
		FRotator SpawnRot = Character->GetActorRotation();
		if (IsValid(CharacterMesh))
		{
			FActorSpawnParameters Params;
			Params.Owner = GetAvatarActorFromActorInfo();
			Params.Instigator = Cast<APawn>(Params.Owner);

			SpawnedFlash = World->SpawnActorDeferred<ABaseProjectile>(
				FlashClass,
				FTransform(SpawnRot, SpawnLoc),
				Character,
				Character,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn
				);
		}
		UGameplayStatics::FinishSpawningActor(SpawnedFlash, FTransform(SpawnRot, SpawnLoc));
		// Attach Flash
		if (IsValid(SpawnedFlash))
		{
			SpawnedFlash->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("FlashSocket"));
			SpawnedFlash->SetActorEnableCollision(false);
		}
	}

	UGameplayStatics::FinishSpawningActor(SpawnedActor, SpawnTransform);
	
	UAbilityTask_WaitTargetData* WaitTargetData = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(
		this,
		FName("Flash"),
		EGameplayTargetingConfirmation::UserConfirmed,
		SpawnedActor
	);
	
	WaitTargetData->ValidData.AddDynamic(this, &UFlash::OnTargetDataReady_Internal);
	WaitTargetData->Cancelled.AddDynamic(this, &UFlash::OnTargetCancelled_Internal);
	WaitTargetData->ReadyForActivation();
}

void UFlash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UFlash::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	SpawnedFlash->Destroy();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}


void UFlash::OnTargetDataReady_Internal(const FGameplayAbilityTargetDataHandle& Data)
{
	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	OnTargetDataReady(Data);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UFlash::OnTargetCancelled_Internal(const FGameplayAbilityTargetDataHandle& Data)
{
	//CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	OnTargetCancelled(Data);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UFlash::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& Data)
{
	//FLogger::DebugLog("Left click Flash");
}

void UFlash::OnTargetCancelled(const FGameplayAbilityTargetDataHandle& Data)
{
	//FLogger::DebugLog("Right click Flash");
}


