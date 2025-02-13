// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GoblinCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GruntGoblin.generated.h"

/**
 * The Grunt Goblin is the most basic and common goblin type
 */
UCLASS()
class FPSPROJECT_API AGruntGoblin : public AGoblinCharacter
{
	GENERATED_BODY()
	
public:
	AGruntGoblin();
};
