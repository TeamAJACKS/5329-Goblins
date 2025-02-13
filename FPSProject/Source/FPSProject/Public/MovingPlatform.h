// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Engine/StaticMesh.h"
#include "MovingPlatform.generated.h"

UCLASS(hidecategories = ("Actor", "Input", "HLOD"))
class FPSPROJECT_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovingPlatform();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void Move(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
private:
	USceneComponent* Root = nullptr;

	UPROPERTY(EditInstanceOnly)
		UStaticMeshComponent* StaticMeshComponent = nullptr;
	UPROPERTY(EditInstanceOnly)
		UStaticMesh* Platform = nullptr;

	UPROPERTY(EditInstanceOnly)
		UBoxComponent* TriggerBoxComponent = nullptr;

	UPROPERTY(EditInstanceOnly)
		UTimelineComponent* Timeline = nullptr;
	UPROPERTY(EditInstanceOnly)
		UCurveFloat* CurveFloat = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "Sound", meta = (ToolTip = "Which sound should the moving platform play while moving?"))
		USoundBase* MovingPlatformSound = nullptr;

	FVector CurrentLocation = FVector(0, 0, 0);
	FVector EndLocation = FVector(0, 0, 0);

	FRotator CurrentRotation = FRotator(0, 0, 0);
	FRotator EndRotation = FRotator(0, 0, 0);

	UPROPERTY(EditInstanceOnly, Category = "Move", meta = (ToolTip = "How many units should the platform move on the Z-Axis?"))
		float ZMove = 500;
	UPROPERTY(EditInstanceOnly, Category = "Move", meta = (ToolTip = "How many units should the platform rotate on the Yaw?"))
		float RotateYaw = 150;
	UPROPERTY(EditInstanceOnly, Category = "Move", meta = (ToolTip = "How fast should the platform move? (0=No movement 1=Normal 2+=Fast)"))
		float Speed = 1;
};
