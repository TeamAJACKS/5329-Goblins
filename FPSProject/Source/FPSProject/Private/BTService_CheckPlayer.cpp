// Copyright © 2018, AJACKS

#include "BTService_CheckPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"
#include "FirstPersonCharacter.h"
#include "GoblinCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_CheckPlayer::UBTService_CheckPlayer()
{
	bCreateNodeInstance = true;
	bNotifyBecomeRelevant = true;

	static ConstructorHelpers::FObjectFinder<UUserDefinedEnum> GoblinAIStateEnum(TEXT("UserDefinedEnum'/Game/5329_Goblins/AI/GoblinAIState.GoblinAIState'"));

	if (GoblinAIStateEnum.Succeeded())
		GoblinAIState = GoblinAIStateEnum.Object;
}

void UBTService_CheckPlayer::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Set Goblin AI state to Idle
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(FName(StateKey.SelectedKeyName), GoblinAIState->GetValueByIndex(0));
}

void UBTService_CheckPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Get a reference to our characters and AI Controller
	const auto PlayerCharacter = Cast<AFirstPersonCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	const auto GoblinCharacter = Cast<AGoblinCharacter>(OwnerComp.GetAIOwner()->GetCharacter());

	// Calculate a line to the player
	FVector LineToPlayer = PlayerCharacter->GetActorLocation() - GoblinCharacter->GetActorLocation();
	LineToPlayer.Normalize();

	// Can see the player?
	// Left value - Find angle to player in degrees
	if (FMath::Acos(FVector::DotProduct(LineToPlayer, GoblinCharacter->GetActorForwardVector())) < SightInDegrees)
	{
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GoblinCharacter->GetCapsuleComponent()->GetOwner());

		// Sight
		if (bDrawDebugLineToPlayer)
			DrawDebugLine(GetWorld(), GoblinCharacter->GetActorLocation() + FVector(0, 0, 60), PlayerCharacter->GetActorLocation() + FVector(0, 0, 60), FColor::Red, false, 1.0f, 0, 1.0f);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, GoblinCharacter->GetActorLocation() + FVector(0, 0, 60), PlayerCharacter->GetActorLocation() + FVector(0, 0, 60), ECC_Camera, QueryParams))
		{
			const auto HitActor = Cast<AActor>(HitResult.GetActor());

			if (HitActor && HitActor == UGameplayStatics::GetPlayerCharacter(this, 0))
			{
				// Set Goblin AI state to Attack
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(FName(StateKey.SelectedKeyName), GoblinAIState->GetValueByIndex(1));

				// Set the target actor to move to
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(GoalActorKey.SelectedKeyName), PlayerCharacter);

				if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(MemoryMarkerKey.SelectedKeyName)) != nullptr)
				{
					auto MemoryMarkerActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(MemoryMarkerKey.SelectedKeyName)));
					MemoryMarkerActor->SetActorLocation(PlayerCharacter->GetActorLocation());
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(MemoryMarkerKey.SelectedKeyName), TargetPoint);
					
					if (bDrawDebugMemoryMarker)
						DrawDebugSphere(GetWorld(), MemoryMarkerActor->GetActorLocation(), 50.0f, 10, FColor::Red, false, 1.0f, 0, 1.0f);

					// Set Goblin AI state to Attack
					OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(FName(StateKey.SelectedKeyName), GoblinAIState->GetValueByIndex(1));
				}
				else
				{
					if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(MemoryMarkerKey.SelectedKeyName)) != nullptr)
					{
						// Set Goblin AI state to Searching
						OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(FName(StateKey.SelectedKeyName), GoblinAIState->GetValueByIndex(2));
					}
					else
					{
						// Set Goblin AI state to Idle
						OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(FName(StateKey.SelectedKeyName), GoblinAIState->GetValueByIndex(0));
					}

					// Spawn the memory markers and set in BB
					TargetPoint = GetWorld()->SpawnActor<ATargetPoint>(ATargetPoint::StaticClass(), PlayerCharacter->GetActorTransform());
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(MemoryMarkerKey.SelectedKeyName), TargetPoint);
				}
			}
			else
			{
				if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(MemoryMarkerKey.SelectedKeyName)) != nullptr)
				{
					// Set Goblin AI state to Searching
					OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(FName(StateKey.SelectedKeyName), GoblinAIState->GetValueByIndex(2));
				}
				else
				{
					// Set Goblin AI state to Idle
					OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(FName(StateKey.SelectedKeyName), GoblinAIState->GetValueByIndex(0));
				}
			}
		}
	}
	else
	{
		if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(MemoryMarkerKey.SelectedKeyName)) != nullptr)
		{
			// Set Goblin AI state to Searching
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(FName(StateKey.SelectedKeyName), GoblinAIState->GetValueByIndex(2));
		}
		else
		{
			// Set Goblin AI state to Idle
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(FName(StateKey.SelectedKeyName), GoblinAIState->GetValueByIndex(0));
		}
	}
}

void UBTService_CheckPlayer::OnGameplayTaskActivated(UGameplayTask& Task)
{
	
}

void UBTService_CheckPlayer::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	
}

