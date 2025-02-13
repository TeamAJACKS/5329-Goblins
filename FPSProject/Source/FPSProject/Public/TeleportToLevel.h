// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "TeleportToLevel.generated.h"

UCLASS(hidecategories = ("Rendering","Actor", "Input", "HLOD"))
class FPSPROJECT_API ATeleportToLevel : public AActor
{
	GENERATED_BODY()
	
public:	
	ATeleportToLevel();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void FadeOutScreen(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void Teleport(FString InMapName) const;

	UPROPERTY(EditInstanceOnly)
	UBoxComponent* TriggerBoxComponent = nullptr; // The trigger to teleport to level

	UPROPERTY(EditInstanceOnly, Category = "Level", meta = (ToolTip = "The name of the map to teleport to when player overlaps with the trigger box"))
	FString LevelToTeleportTo = FString("BossLevel");

private:
	FTimerHandle TeleportDelayTimerHandle;
	FTimerDelegate TeleportDelayTimerDel;

	UWorld* NewLevel = nullptr;
};
