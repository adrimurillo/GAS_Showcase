// Fill out your copyright notice in the Description page of Project Settings.


#include "AEnemyCharacter.h"

#include "GASRPG/GameplayAbilitySystem/Attributes/BasicAttributeSet.h"

AAEnemyCharacter::AAEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

void AAEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AAEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


