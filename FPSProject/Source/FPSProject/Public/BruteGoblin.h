// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GoblinCharacter.h"
#include "BruteGoblin.generated.h"

/**
 * The boss goblin
 */
UCLASS()
class FPSPROJECT_API ABruteGoblin : public AGoblinCharacter
{
	GENERATED_BODY()
	
public:
	ABruteGoblin();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UMaterialInterface* BruteGoblinMaterial = nullptr;
};
