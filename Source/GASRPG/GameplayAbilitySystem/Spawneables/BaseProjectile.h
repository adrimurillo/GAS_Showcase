#pragma once

#include "CoreMinimal.h"
#include "BaseSpawnable.h"
#include "BaseProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class GASRPG_API ABaseProjectile : public ABaseSpawnable
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flash", meta = (AllowPrivateAccess = true))
	UProjectileMovementComponent* ProjectileMovement;

public:
	ABaseProjectile();

	virtual void LaunchProjectile(const FVector& Velocity);

protected:
	virtual void BeginPlay() override;

};
