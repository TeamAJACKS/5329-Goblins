// Copyright © 2018, AJACKS

#include "GoblinAIController.h"
#include "GoblinCharacter.h"
#include "Engine/Engine.h"
#include "FirstPersonCharacter.h"


AGoblinAIController::AGoblinAIController()
{
	// Blackboard Component
	BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(FName("Blackboard Component"));

	// Behaviour Tree Component
	BehaviourTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(FName("Behaviour Tree Component"));

	// Perception Component
	//GoblinPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(FName("AI Perception"));
	//SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	//GoblinPerceptionComponent->ConfigureSense(*SightConfig);
	//GoblinPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	//GoblinPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AGoblinAIController::SenseUpdated);

	// Initialize Key Names
	KeyNames[0] = "Target";
	//KeyNames[1] = "SelfActor";
	KeyNames[2] = "RandomLocation";
	KeyNames[3] = "";
	KeyNames[4] = "";

	// Set keys
	for (uint8 i = 0; i < MaxKeys; i++)
		KeyID[i] = BlackBoardComponent->GetKeyID(KeyNames[i]);
}

void AGoblinAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	// Perception Setup
	//SightConfig->SightRadius = 1200.0f;
	//SightConfig->LoseSightRadius = SightConfig->SightRadius + 700.0f;
	//SightConfig->PeripheralVisionAngleDegrees = 50.0f;
	//SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	//SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	//SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	//GoblinPerceptionComponent->ConfigureSense(*SightConfig);

	// Initialize all blackboard keys and start
	AGoblinCharacter* GoblinCharacter = Cast<AGoblinCharacter>(InPawn);
	if (GoblinCharacter && GoblinCharacter->GoblinBehavior)
	{
		BlackBoardComponent->InitializeBlackboard(*GoblinCharacter->GoblinBehavior->BlackboardAsset);

		// Store blackboard keyIds for use in the Behaviour tree services
		for (uint8 i = 0; i < MaxKeys; i++)
			KeyID[i] = BlackBoardComponent->GetKeyID(KeyNames[i]);

		BehaviourTreeComponent->StartTree(*GoblinCharacter->GoblinBehavior);
	}
}

void AGoblinAIController::SenseUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	//if (Stimulus.WasSuccessfullySensed())
	//{
	//	BlackBoardComponent->SetValueAsObject(FName("Target"), Actor);
	//	AIStimulus = Stimulus;
	//	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString("I see you! ") + FString("says: ") + GetPawn()->GetName());
	//}
	//else
	//{
	//	BlackBoardComponent->SetValueAsObject(FName("Target"), nullptr);
	//	AIStimulus = Stimulus;
	//	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString("I don't see you! ") + FString("says: ") + GetPawn()->GetName());
	//}
}


