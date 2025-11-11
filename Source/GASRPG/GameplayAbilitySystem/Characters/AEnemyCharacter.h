#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "AEnemyCharacter.generated.h"

UCLASS()
class GASRPG_API AAEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	

public:
	AAEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
