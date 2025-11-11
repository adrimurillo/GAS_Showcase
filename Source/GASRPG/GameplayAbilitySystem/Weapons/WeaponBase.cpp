// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"


AWeaponBase::AWeaponBase()
{
	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshComponent"));
	RootComponent = WeaponMeshComponent;
	bReplicates = true;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

