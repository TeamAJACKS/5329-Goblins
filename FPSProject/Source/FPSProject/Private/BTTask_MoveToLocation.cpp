// Copyright © 2018, AJACKS

#include "BTTask_MoveToLocation.h"
#include "Kismet/GameplayStatics.h"
#include "GoblinAIController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Engine/Engine.h"


EBTNodeResult::Type UBTTask_MoveToLocation::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	// Get the AI Controller
	auto GoblinAIController = Cast<AGoblinAIController>(OwnerComp.GetAIOwner());

	// Get the nav system in the current world
	UNavigationSystem* NavSystem = GetNavigationSystem();
	FNavLocation NavLocation;

	NavSystem->GetRandomPoint(NavLocation, NavSystem->MainNavData);
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(RandomLocationKey.SelectedKeyName, NavLocation.Location);

	if (bShowDebugRandomLocation)
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString(NavLocation.Location.ToString()));

	const FPathFollowingResult PathFollowingResult;
	FPathFollowingRequestResult RequestResult;
	RequestResult.Code = GoblinAIController->MoveToLocation(NavLocation.Location, 10.0f, true, true, true, true, nullptr, true);
	
	GoblinAIController->OnMoveCompleted(RequestResult.MoveId, PathFollowingResult);
	
	if (RequestResult.Code == EPathFollowingRequestResult::RequestSuccessful)
	{
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Succeeded;
}

UNavigationSystem* UBTTask_MoveToLocation::GetNavigationSystem() const
{
	UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(GetWorld());
	if(!NavSys) 
	{
		// Could not get Nav system
		return nullptr;
	}

	return NavSys;
}

