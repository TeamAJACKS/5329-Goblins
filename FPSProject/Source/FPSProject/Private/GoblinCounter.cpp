// Copyright © 2018, AJACKS

#include "GoblinCounter.h"
#include "../../../../../../../../../Program Files/Epic Games/UE_4.18/Engine/Plugins/MovieScene/ActorSequence/Source/ActorSequence/Private/ActorSequencePrivatePCH.h"


AGoblinCounter::AGoblinCounter()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AGoblinCounter::BeginPlay()
{
	Super::BeginPlay();

	// Get all instances of Goblin Spawners in the world and store in array
	for (TActorIterator<AGoblinSpawner> SpawnerItr(GetWorld()); SpawnerItr; ++SpawnerItr)
	{
		AGoblinSpawner* Spawner = *SpawnerItr;
		Spawners.Add(Spawner);
	}

	// Get the number of goblins to spawn from each spawner instance and store the value in NumOfGoblins
	for (int32 i = 0; i < Spawners.Num(); i++)
	{
		NumOfGoblins += Spawners[i]->Goblins;
	}

	if (bShowDebugMessage)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString("There are ") + FString::FromInt(NumOfGoblins) + FString(" Goblins in the world."));
}

