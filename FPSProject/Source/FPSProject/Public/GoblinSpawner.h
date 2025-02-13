// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "GoblinSpawner.generated.h"

class AGoblinCharacter;

UCLASS(hidecategories = ("Rendering","Actor", "Input", "HLOD"))
class FPSPROJECT_API AGoblinSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AGoblinSpawner();

	UFUNCTION()
	void SpawnGoblin();

	UPROPERTY(EditInstanceOnly, Category = "Spawner Settings", meta = (ToolTip = "How many goblins to spawn?"))
	uint16 Goblins = 10;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void StartSpawner(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:
	USceneComponent *Root = nullptr;

	UPROPERTY(EditInstanceOnly)
	UBoxComponent* TriggerBoxComponent = nullptr; // The trigger to make goblins spawn

	FTimerHandle Handle;

	UPROPERTY(EditInstanceOnly, Category = "Spawner Settings", meta = (ToolTip = "How long to wait before another goblin spawns?"))
	float SpawnDelay = 1.0f;

	UPROPERTY(EditInstanceOnly, Category = "Spawner Settings", meta = (ToolTip = "What type of goblin to spawn?"))
	TSubclassOf<AGoblinCharacter> GoblinType;

	UPROPERTY(VisibleInstanceOnly, Category = "Debug", meta = (ToolTip = "Total Goblins in the world"))
	uint16 GoblinCount = 0;
};
