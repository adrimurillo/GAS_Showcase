#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSpawnable.generated.h"

UCLASS()
class GASRPG_API ABaseSpawnable : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawneable", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

public:
	ABaseSpawnable();

	FORCEINLINE UStaticMeshComponent* GetStaticMesh() const { return StaticMesh; }
protected:
	virtual void BeginPlay() override;

};
