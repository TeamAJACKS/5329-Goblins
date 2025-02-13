// Copyright © 2018, AJACKS

#include "GoblinSpawner.h"
#include "GoblinCharacter.h"
#include "Kismet/GameplayStatics.h"


AGoblinSpawner::AGoblinSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	// Scene Component
	Root = CreateDefaultSubobject<USceneComponent>(FName("Scene Component"));
	Root = RootComponent;

	// Trigger Box Component
	TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box Component"));
	TriggerBoxComponent->SetupAttachment(Root);
	TriggerBoxComponent->SetWorldLocation(FVector(0, 0, 0));
	TriggerBoxComponent->SetBoxExtent(FVector(50, 40, 50));
	TriggerBoxComponent->SetCollisionProfileName(FName("Trigger"));
	TriggerBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AGoblinSpawner::StartSpawner);
}

void AGoblinSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void AGoblinSpawner::StartSpawner(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// If OtherActor is valid AND if the OtherActor is not of this class (SlidingDoor) AND is a PlayerCharacter
	if (OtherActor && (OtherActor != this) && OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		GetWorldTimerManager().SetTimer(Handle, this, &AGoblinSpawner::SpawnGoblin, SpawnDelay, true);
		TriggerBoxComponent->DestroyComponent();
	}
}

void AGoblinSpawner::SpawnGoblin()
{
	GetWorld()->SpawnActor<AGoblinCharacter>(GoblinType, GetActorLocation(), GetActorRotation());
	GoblinCount++;

	if (GoblinCount == Goblins)
		GetWorldTimerManager().ClearTimer(Handle);
}

