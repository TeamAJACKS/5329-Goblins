// Copyright © 2018, AJACKS

#include "BTTask_Attack.h"
#include "GoblinAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"


UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;

	static ConstructorHelpers::FObjectFinder<UUserDefinedEnum> GoblinAIStateEnum(TEXT("UserDefinedEnum'/Game/5329_Goblins/AI/GoblinAIState.GoblinAIState'"));

	if (GoblinAIStateEnum.Succeeded())
		GoblinAIState = GoblinAIStateEnum.Object;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto GoblinController = Cast<AGoblinAIController>(OwnerComp.GetAIOwner());
	GoblinCharacter = Cast<AGoblinCharacter>(GoblinController->GetCharacter());
	PlayerCharacter = Cast<AFirstPersonCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")));

	GoblinCharacter->GoblinAnimInstance->bCanAttack = true;
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	if (PlayerCharacter->CurrentHealth <= 0)
	{
		GoblinCharacter->GoblinAnimInstance->bCanAttack = false;

		// Set Goblin AI state to Idle
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(FName(BlackboardKey.SelectedKeyName), GoblinAIState->GetValueByIndex(0));

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
