// Copyright © 2018, AJACKS

#include "BTTask_DestroyMemory.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "ConstructorHelpers.h"


UBTTask_DestroyMemory::UBTTask_DestroyMemory()
{
	static ConstructorHelpers::FObjectFinder<UUserDefinedEnum> GoblinAIStateEnum(TEXT("UserDefinedEnum'/Game/5329_Goblins/AI/GoblinAIState.GoblinAIState'"));

	if (GoblinAIStateEnum.Succeeded())
		GoblinAIState = GoblinAIStateEnum.Object;
}


EBTNodeResult::Type UBTTask_DestroyMemory::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* MemoryMarkerActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(MemoryMarkerKey.SelectedKeyName)));
	MemoryMarkerActor->Destroy();

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(StateKey.SelectedKeyName), GoblinAIState->GetValueByIndex(0));

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

