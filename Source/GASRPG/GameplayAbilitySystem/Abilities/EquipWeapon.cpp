#include "EquipWeapon.h"

#include "AbilitySystemComponent.h"
#include "GASRPG/GASRPG.h"
#include "GASRPG/GameplayAbilitySystem/Weapons/BattleAxe.h"
#include "GASRPG/GameplayAbilitySystem/Weapons/WeaponManager.h"


void UEquipWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CommitAbility(Handle, ActorInfo, ActivationInfo);

	const FGameplayAbilitySpec* Spec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
	if (!Spec)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	EGASAbilityInputID InputPressed = static_cast<EGASAbilityInputID>(Spec->InputID);

	UWeaponManager* WeaponManager = Cast<UWeaponManager>(GetAvatarActorFromActorInfo()->GetComponentByClass(UWeaponManager::StaticClass()));
	if (!IsValid(WeaponManager))
	{
		return;
	}

	switch (InputPressed)
	{
	case EGASAbilityInputID::Weapon1:
		WeaponManager->EquipWeapon(EWeaponSlot::Primary);
		break;
	case EGASAbilityInputID::Weapon2:
		WeaponManager->EquipWeapon(EWeaponSlot::Secondary);
		break;
	case EGASAbilityInputID::Weapon3:
		WeaponManager->EquipWeapon(EWeaponSlot::Melee);
		break;
	default:
		break;
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UEquipWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

