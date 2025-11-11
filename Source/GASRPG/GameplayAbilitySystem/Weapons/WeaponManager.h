#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Components/ActorComponent.h"
#include "WeaponManager.generated.h"


class ABaseCharacter;
class AWeaponBase;

UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	Primary     UMETA(DisplayName = "Primary"),
	Secondary   UMETA(DisplayName = "Secondary"),
	Melee       UMETA(DisplayName = "Melee")
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GASRPG_API UWeaponManager : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	ABaseCharacter* Owner;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = true))
	TMap<EWeaponSlot, TSubclassOf<AWeaponBase>> UnlockedWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = true))
	TMap<EWeaponSlot, AWeaponBase*> SpawnedWeapons;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = true), ReplicatedUsing=OnRep_EquippedWeapon)
	AWeaponBase* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = true), ReplicatedUsing=OnRep_EquippedWeapon)
	AWeaponBase* LastEquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = true))
	FWeaponConfig DefaultConfig;


public:
	UWeaponManager();

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(EWeaponSlot Weapon);
	
	UFUNCTION(BlueprintCallable)
	void UnEquipWeapon();

	UFUNCTION(BlueprintCallable)
	void InitializeWeapons();

	void SpawnWeapons(EWeaponSlot WeaponSlot, const TSubclassOf<AWeaponBase>& WeaponClass);

	//UFUNCTION(BlueprintCallable)
	//FORCEINLINE TMap<TSubclassOf<AWeaponBase>, AWeaponBase*> GetUnlockedWeapons() const { return UnlockedWeapons; }

	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&	OutLifetimeProps) const override;
protected:
	virtual void BeginPlay() override;

private:
	void SetWeaponProperties(const FWeaponConfig& WeaponConfig);

	UFUNCTION()
	void OnRep_EquippedWeapon();

};
