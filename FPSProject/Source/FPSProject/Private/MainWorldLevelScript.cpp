// Copyright © 2018, AJACKS

#include "MainWorldLevelScript.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/Engine.h"


void AMainWorldLevelScript::BeginPlay()
{
	// Fade in from black
	UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(2.0f, 0.0f, 5.0f, FColor::Black);
}

void AMainWorldLevelScript::OnPlayerDeath() const
{
	// Fade out to black
	UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(0.0f, 2.0f, 1.0f, FColor::Black, true, true);
	
	// Restart level after 2 seconds
	FTimerHandle RestartLevelTimerHandle;
	GetWorldTimerManager().SetTimer(RestartLevelTimerHandle, this, &AMainWorldLevelScript::RestartLevel, 2.0f, false);
}

void AMainWorldLevelScript::RestartLevel() const
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));

	if (bShowDebugLevelRestartMessage)
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString("Level restarted"));
}
