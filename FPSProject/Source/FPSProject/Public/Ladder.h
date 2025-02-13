// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Classes/GameFramework/CharacterMovementComponent.h"
#include "Engine/StaticMesh.h"
#include "Ladder.generated.h"

UCLASS(hidecategories = ("Actor", "Input", "HLOD"))
class FPSPROJECT_API ALadder : public AActor
{
	GENERATED_BODY()
	
public:	
	ALadder();

protected:
	virtual void BeginPlay() override;

	// Turn on/off climbing mode when we have entered the trigger box
	UFUNCTION()
		void ToggleClimbOn(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
		void ToggleClimbOff(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// For use with the floor trigger box component
	UFUNCTION()
		void DisableClimbMode(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void MoveForward(float Scale);
	virtual void MoveRight(float Scale);

private:
	USceneComponent* Root = nullptr;

	UPROPERTY(EditInstanceOnly)
		UStaticMeshComponent* StaticMeshComponent = nullptr;
	UPROPERTY(EditInstanceOnly)
		UStaticMesh* Ladder = nullptr; // The ladder mesh

	UPROPERTY(EditInstanceOnly)
		UBoxComponent* ClimbingTriggerBoxComponent = nullptr; // To trigger climbing functionality 
	UPROPERTY(EditInstanceOnly)
		UBoxComponent* FloorTriggerBoxComponent = nullptr; // To trigger walk mode when the player climbs down the ladder and hits the floor

	APawn* OurPawn = nullptr; // A reference to access our player in the world so we can trigger the flying/walk up movement mode
	UCharacterMovementComponent* CharacterMovement = nullptr; // A reference to access and set the movement mode if we are climbing or not

	bool CanClimb = false; // To check if we can set the movement input to walk up or forward if we are climbing or not.
};
