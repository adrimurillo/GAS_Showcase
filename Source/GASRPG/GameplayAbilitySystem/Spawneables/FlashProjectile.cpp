#include "FlashProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GASRPG/GameplayAbilitySystem/Characters/APlayerCharacter.h"
#include "GASRPG/Util/LoggerUtil.h"
#include "Kismet/KismetSystemLibrary.h"

AFlashProjectile::AFlashProjectile()
{
	LifeTime = 2.f;
	FlashRadius = 6000.f;
	FlashDuration = 2.f;
	FlashMinDotProduct = -0.7f;
}

void AFlashProjectile::LaunchProjectile(const FVector& Velocity)
{
	Super::LaunchProjectile(Velocity);

	FTimerHandle ExplodeTimer;

	GetWorld()->GetTimerManager().SetTimer(ExplodeTimer, this, &AFlashProjectile::Explode, LifeTime);
}

void AFlashProjectile::Explode()
{
	TArray<AActor*> OutActors;
	UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), FlashRadius, { ObjectTypeQuery3 }, AAPlayerCharacter::StaticClass(), {}, OutActors);
	DrawDebugSphere(GetWorld(), GetActorLocation(), FlashRadius, 16, FColor::Red);

	for (AActor* OutActor : OutActors)
	{
		UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OutActor);
		if (IsValid(AbilitySystemComponent) && IsValid(FlashEffect))
		{
			FHitResult Hit;
			UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), OutActor->GetActorLocation(), TraceTypeQuery2, false, { this }, EDrawDebugTrace::None, Hit, true);

			if (!Hit.bBlockingHit || Hit.GetActor() != OutActor && Hit.GetActor()->GetOwner() != OutActor)
			{
				continue;
			}
			const float Dot = FVector::DotProduct(OutActor->GetActorForwardVector(), (OutActor->GetActorLocation() - GetActorLocation()).GetSafeNormal());
			if (Dot < FlashMinDotProduct)
			{
				FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(FlashEffect, 1, AbilitySystemComponent->MakeEffectContext());
				EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FlashDurationTag, FlashDuration);
				AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
			}
		}
	}

	Destroy();
}

void AFlashProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}


