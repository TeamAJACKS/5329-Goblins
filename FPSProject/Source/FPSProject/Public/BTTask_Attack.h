// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FirstPersonCharacter.h"
#include "GoblinCharacter.h"
#include "Engine/UserDefinedEnum.h"
#include "BTTask_Attack.generated.h"

/**
 * Attacks the player when stopped at an acceptance radius
 */
UCLASS()
class FPSPROJECT_API UBTTask_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UUserDefinedEnum* GoblinAIState = nullptr;

	AFirstPersonCharacter* PlayerCharacter = nullptr;
	AGoblinCharacter* GoblinCharacter = nullptr;
};
