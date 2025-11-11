#include "WeaponManager.h"

#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GASRPG/GameplayAbilitySystem/Characters/BaseCharacter.h"
#include "GASRPG/Util/LoggerUtil.h"
#include "Net/UnrealNetwork.h"

UWeaponManager::UWeaponManager()
{
	LastEquippedWeapon = nullptr;
}

void UWeaponManager::EquipWeapon(const EWeaponSlot Weapon)
{
	if (!Owner->HasAuthority())
	{
		return;
	}

	if (!SpawnedWeapons.Contains(Weapon))
	{
		return;
	}
	
	if (SpawnedWeapons[Weapon] == EquippedWeapon)
	{
		return;
	}
	UnEquipWeapon();

	EquippedWeapon = SpawnedWeapons[Weapon];
	EquippedWeapon->SetActorHiddenInGame(false);
	SetWeaponProperties(EquippedWeapon->GetWeaponConfig());
}


void UWeaponManager::UnEquipWeapon()
{
	if (!IsValid(EquippedWeapon))
	{
		return;
	}
	SetWeaponProperties(DefaultConfig);
	EquippedWeapon->SetActorHiddenInGame(true);
	EquippedWeapon = nullptr;
}


void UWeaponManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponManager, EquippedWeapon, COND_None, REPNOTIFY_Always);
}

void UWeaponManager::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ABaseCharacter>(GetOwner());

	InitializeWeapons();
}

void UWeaponManager::InitializeWeapons()
{
	if (!IsValid(Owner))
	{
		return;
	}
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		FLogger::ErrorLog("[UWeaponManager][InitializeWeapons] World is invalid!]");
	}

	for (auto& Pair: UnlockedWeapons)
	{
		SpawnWeapons(Pair.Key, Pair.Value);
	}
	EquipWeapon(EWeaponSlot::Melee);
}

void UWeaponManager::SpawnWeapons(const EWeaponSlot WeaponSlot, const TSubclassOf<AWeaponBase>& WeaponClass)
{
	if (!IsValid(WeaponClass))
	{
		return;
	}

	AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);
	if (!IsValid(Weapon))
	{
		return;
	}
	Weapon->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Weapon->GetWeaponConfig().EquippedSocketName);
	Weapon->SetActorHiddenInGame(true);
	SpawnedWeapons.Add(WeaponSlot, Weapon);
}

void UWeaponManager::SetWeaponProperties(const FWeaponConfig& WeaponConfig)
{
	if (!IsValid(Owner) || !IsValid(Owner->GetCharacterMovement()))
	{
		return;
	}
	Owner->GetCharacterMovement()->MaxWalkSpeed = WeaponConfig.MovementProperties.MaxWalkSpeed;
	Owner->GetCharacterMovement()->bOrientRotationToMovement = WeaponConfig.MovementProperties.OrientRotationToMovement;
	Owner->GetCharacterMovement()->bUseControllerDesiredRotation = WeaponConfig.MovementProperties.UseControllerDesiredRotation;
	if (IsValid(Owner) && IsValid(WeaponConfig.AnimClass))
	{
		Owner->GetMesh()->SetAnimInstanceClass(WeaponConfig.AnimClass);
	}
}

void UWeaponManager::OnRep_EquippedWeapon()
{
	if (IsValid(LastEquippedWeapon) && LastEquippedWeapon != EquippedWeapon)
	{
		LastEquippedWeapon->SetActorHiddenInGame(true);
	}

	if (IsValid(EquippedWeapon))
	{
		EquippedWeapon->SetActorHiddenInGame(false);
		SetWeaponProperties(EquippedWeapon->GetWeaponConfig());
	}
	else
	{
		SetWeaponProperties(DefaultConfig);
	}

	LastEquippedWeapon = EquippedWeapon;
}

