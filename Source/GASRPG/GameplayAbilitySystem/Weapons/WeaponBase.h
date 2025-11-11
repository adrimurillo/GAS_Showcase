#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FWeaponMovementProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Properties")
	float MaxWalkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Properties")
	bool OrientRotationToMovement;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Properties")
	bool UseControllerDesiredRotation;
};

USTRUCT(BlueprintType)
struct FWeaponConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FName EquippedSocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<UAnimInstance> AnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FWeaponMovementProperties MovementProperties;
};

UCLASS()
class GASRPG_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:
	AWeaponBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components", meta = (AllowPrivateAccess))
	UStaticMeshComponent* WeaponMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config", meta = (AllowPrivateAccess))
	FWeaponConfig WeaponConfig;


public:
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return WeaponMeshComponent; }
	FORCEINLINE FWeaponConfig GetWeaponConfig() const { return WeaponConfig; }
protected:
	virtual void BeginPlay() override;

};
