#include "EffectAreaBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GASRPG/Util/LoggerUtil.h"


AEffectAreaBase::AEffectAreaBase()
	: AreaRadius(100.f)
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComp->SetSphereRadius(AreaRadius);
	RootComponent = SphereComp;
	bReplicates = true;
	
}

void AEffectAreaBase::SetAreaRadius(const float Radius)
{
	SphereComp->SetSphereRadius(Radius);
}

void AEffectAreaBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEffectAreaBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (!HasAuthority())
	{
		return;
	}
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (!IsValid(TargetASC))
	{
		return;
	}

	for (FEffectProperties EffectProperty : EffectProperties)
	{
		if (!IsValid(EffectProperty.GameplayEffectClass))
		{
			continue;
		}
		FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(EffectProperty.GameplayEffectClass, 1.f, TargetASC->MakeEffectContext());
		SpecHandle.Data->SetSetByCallerMagnitude(EffectProperty.DataTag, EffectProperty.Magnitude);
		if (SpecHandle.IsValid())
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AEffectAreaBase::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (!HasAuthority())
	{
		return;
	}
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (!IsValid(TargetASC))
	{
		return;
	}
	
	for (FEffectProperties EffectProperty : EffectProperties)
	{
		TargetASC->RemoveActiveGameplayEffectBySourceEffect(EffectProperty.GameplayEffectClass, TargetASC);
		//FTimerHandle TimerHandle;
		//GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, TargetASC, EffectProperty]() -> void
		//{
		//	
		//}, EffectProperty.EffectDurationTimeAfterEndOverlap, false);
	}
	
}


