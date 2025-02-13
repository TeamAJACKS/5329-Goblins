// Copyright © 2018, AJACKS

#include "BTTask_SetWalkSpeed.h"
#include "GoblinAIController.h"
#include "GoblinCharacter.h"
#include "GruntGoblin.h"
#include "GameFramework/CharacterMovementComponent.h"


EBTNodeResult::Type UBTTask_SetWalkSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI Controller
	const auto GoblinAIController = Cast<AGoblinAIController>(OwnerComp.GetAIOwner());
	const auto Goblin = GoblinAIController->GetCharacter();

	if (Goblin->IsA(AGoblinCharacter::StaticClass()))
	{
		GoblinAIController->GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

		return EBTNodeResult::Succeeded;
	}
	
	if (Goblin->IsA(AGruntGoblin::StaticClass()))
	{
		GoblinAIController->GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

