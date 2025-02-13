// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "HealthPickup.generated.h"

UCLASS(hidecategories = ("Input", "Actor", "HLOD"))
class FPSPROJECT_API AHealthPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	AHealthPickup();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Pickup(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void Float(float DeltaTime);
	void Rotate(float DeltaTime);

private:
	// Components
	USceneComponent* Root = nullptr;

	UPROPERTY(EditInstanceOnly)
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UStaticMesh* FirstAidKit = nullptr; // The First aid kit mesh

	UPROPERTY(EditInstanceOnly)
	USphereComponent* SphereComponent = nullptr; // The sphere that the player will overlap

	// Variables
	UPROPERTY(EditInstanceOnly, Category = "Float", meta = (ToolTip = "How fast should the First Aid Kit spin? (0=No movement 1=Normal 2+=Fast)"))
	float SpinSpeed = 50.0f;

	UPROPERTY(EditInstanceOnly, Category = "Float", meta = (ToolTip = "How fast should the First Aid Kit float up and down? (0=No movement 1=Normal 2+=Fast)"))
	float FloatingSpeed = 3.0f;

	UPROPERTY(EditInstanceOnly, Category = "Float", meta = (ToolTip = "How high should the First Aid Kit float?"))
	float FloatHeight = 20.0f;

	UPROPERTY(EditInstanceOnly, Category = "Health", meta = (ToolTip = "How much minimum health should the player receive?"))
	int MinAmount = 5;

	UPROPERTY(EditInstanceOnly, Category = "Health", meta = (ToolTip = "How much maximum health should the player receive?"))
	int MaxAmount = 20;

	float RunningTimeLoc = 0.0f;
};
