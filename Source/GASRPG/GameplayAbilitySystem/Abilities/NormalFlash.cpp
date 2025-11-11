#include "NormalFlash.h"

#include "GASRPG/GameplayAbilitySystem/Spawneables/BaseProjectile.h"
#include "GASRPG/Util/LoggerUtil.h"

void UNormalFlash::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& Data)
{
	if (!IsValid(SpawnedFlash))
	{
		return;
	}
	SpawnedFlash->SetActorEnableCollision(true);
	SpawnedFlash->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	FVector LaunchDir;
	if (Data.Get(0)->GetHitResult()->bBlockingHit)
	{
		LaunchDir = (Data.Get(0)->GetEndPoint() - SpawnedFlash->GetActorLocation()).GetSafeNormal();
	}
	else
	{
		LaunchDir = (Data.Get(0)->GetHitResult()->TraceEnd - SpawnedFlash->GetActorLocation()).GetSafeNormal();
	}

	SpawnedFlash->LaunchProjectile(LaunchDir * ThrowForce);
}

void UNormalFlash::OnTargetCancelled(const FGameplayAbilityTargetDataHandle& Data)
{
	if (IsLocallyControlled() && !HasAuthority(&CurrentActivationInfo))
	{
		return;
	}
	
	if (!IsValid(SpawnedFlash))
	{
		return;
	}

	SpawnedFlash->Destroy();
}
