// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "BattleAxe.generated.h"

UCLASS()
class GASRPG_API ABattleAxe : public AWeaponBase
{
	GENERATED_BODY()

public:
	ABattleAxe();

protected:
	virtual void BeginPlay() override;

};
