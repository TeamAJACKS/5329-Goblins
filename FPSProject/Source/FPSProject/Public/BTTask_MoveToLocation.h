// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AI/Navigation/RecastNavMesh.h"
#include "BTTask_MoveToLocation.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UBTTask_MoveToLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UNavigationSystem* GetNavigationSystem() const;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bShowDebugRandomLocation = false;

	UPROPERTY(EditAnywhere, Category = "Keys")
	FBlackboardKeySelector RandomLocationKey;

	TSubclassOf<ARecastNavMesh> NavMeshClass;
	TArray<AActor*> OutNavMeshActor;
};
