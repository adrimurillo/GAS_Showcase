#include "GASPlayerState.h"

#include "AbilitySystemComponent.h"
#include "GASRPG/GameplayAbilitySystem/Attributes/BasicAttributeSet.h"
#include "GASRPG/GameplayAbilitySystem/Attributes/MagicAttributeSet.h"

void AGASPlayerState::BeginPlay()
{
	Super::BeginPlay();

}


AGASPlayerState::AGASPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	BasicAttributeSet = CreateDefaultSubobject<UBasicAttributeSet>(TEXT("BasicAttributeSet"));
	MagicAttributeSet = CreateDefaultSubobject<UMagicAttributeSet>(TEXT("MagicAttributeSet"));

	SetNetUpdateFrequency(30.f);
}

UAbilitySystemComponent* AGASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
