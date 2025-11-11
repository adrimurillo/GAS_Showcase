#include "AGameplayAbilityTargetActor_LinetraceCamera.h"

FHitResult AAGameplayAbilityTargetActor_LinetraceCamera::PerformTrace(AActor* InSourceActor)
{
	FHitResult Hit;
	FVector CamLoc;
	FRotator CamRot;

	if (PrimaryPC)
	{
		PrimaryPC->GetPlayerViewPoint(CamLoc, CamRot);
	}
	else
	{
		InSourceActor->GetActorEyesViewPoint(CamLoc, CamRot);
	}
	
	FVector End = CamLoc + CamRot.Vector() * MaxRange;

	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.AddIgnoredActor(InSourceActor);

	GetWorld()->LineTraceSingleByChannel(Hit, CamLoc, End, ECC_Visibility, Params);
	
	return Hit;
}

void AAGameplayAbilityTargetActor_LinetraceCamera::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FHitResult Hit = PerformTrace(SourceActor);

	if (IsValid(ReticleActor.Get()))
	{
		if (!Hit.bBlockingHit)
		{
			//ReticleActor->SetActorLocation(Hit.TraceEnd);
			return;
		}
		const FVector TargetLocation = Hit.Location;

		const float SmoothSpeed = 5.f;

		FVector NewLocation = FMath::VInterpTo(
			ReticleActor->GetActorLocation(),
			TargetLocation,
			DeltaSeconds,
			SmoothSpeed
		);

		ReticleActor->SetActorLocation(NewLocation);
	}
}
