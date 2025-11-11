#include "BaseProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

ABaseProjectile::ABaseProjectile()
{
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bAutoActivate = false;
}

void ABaseProjectile::LaunchProjectile(const FVector& Velocity)
{
	ProjectileMovement->Velocity = Velocity;
	ProjectileMovement->Activate(); 
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}


