// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Engine/UserDefinedEnum.h"
#include "BTTask_DestroyMemory.generated.h"

/**
 * Destroys the memory marker if AI could not find the player
 */
UCLASS()
class FPSPROJECT_API UBTTask_DestroyMemory : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_DestroyMemory();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Keys")
	FBlackboardKeySelector StateKey;

	UPROPERTY(EditAnywhere, Category = "Keys")
	FBlackboardKeySelector MemoryMarkerKey;

	UUserDefinedEnum* GoblinAIState = nullptr;
};
