#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Abilities/GameplayAbilityTargetActor_SingleLineTrace.h"
#include "AGameplayAbilityTargetActor_LinetraceCamera.generated.h"

UCLASS()
class GASRPG_API AAGameplayAbilityTargetActor_LinetraceCamera : public AGameplayAbilityTargetActor_Trace
{
	GENERATED_BODY()
	
public:
	virtual FHitResult PerformTrace(AActor* InSourceActor) override;
	virtual void Tick(float DeltaSeconds) override;

};
