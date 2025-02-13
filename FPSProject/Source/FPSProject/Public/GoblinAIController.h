// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GoblinAIController.generated.h"

/**
 * Base AI Controller Class for all goblins
 */
UCLASS()
class FPSPROJECT_API AGoblinAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGoblinAIController();

	virtual void Possess(APawn* InPawn) override;
	
	UFUNCTION(Category = "Perception")
		void SenseUpdated(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(EditDefaultsOnly, Category = "Perception")
		TSubclassOf<UAISense> SightSense;

	UAISenseConfig_Sight* SightConfig = nullptr;
	FAIStimulus AIStimulus;

	uint8 KeyID[5];

private:
	UPROPERTY(transient) // transient disables serialization
		UBlackboardComponent* BlackBoardComponent;

	UPROPERTY(transient) // transient disables serialization
		UBehaviorTreeComponent* BehaviourTreeComponent;

	UPROPERTY(transient)
		UAIPerceptionComponent* GoblinPerceptionComponent = nullptr;

	int MaxKeys = 5;
	FName KeyNames[5];
};
