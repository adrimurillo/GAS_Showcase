#include "BaseSpawnable.h"

ABaseSpawnable::ABaseSpawnable()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
}

void ABaseSpawnable::BeginPlay()
{
	Super::BeginPlay();
}


