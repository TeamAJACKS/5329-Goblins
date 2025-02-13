// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextBlock.h"
#include "Components/Image.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "UserInterface", BlueprintReadWrite)
	FString NumOfGoblinsText = FString("0 ") + FString("Goblins Killed");

	UPROPERTY(EditAnywhere, Category = "Animations", BlueprintReadWrite)
	UWidgetAnimation* HealthPickupAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations", BlueprintReadWrite)
	UWidgetAnimation* DamageAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations", BlueprintReadWrite)
	UWidgetAnimation* DeadTextAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animations", BlueprintReadWrite)
	UWidgetAnimation* GoblinsKilledTextAnim;

	UPROPERTY(EditAnywhere, Category = "Animations", BlueprintReadWrite)
	UWidgetAnimation* GoblinsKilledAnim;

	UPROPERTY(EditAnywhere, Category = "Animations", BlueprintReadWrite)
	UWidgetAnimation* CrosshairAnim;

	UPROPERTY(EditAnywhere, Category = "Animations", BlueprintReadWrite)
	UWidgetAnimation* HitMarkerAnim;

	UPROPERTY(EditAnywhere, Category = "Animations", BlueprintReadWrite)
	UWidgetAnimation* KillBarAnim;

	UPROPERTY(EditAnywhere, Category = "Animations", BlueprintReadWrite)
	UWidgetAnimation* KillBarOutlineAnim;

	UPROPERTY(EditAnywhere, Category = "Animations", BlueprintReadWrite)
	UWidgetAnimation* TotalGoblinsAnim;

	UTextBlock* DeadTextBlock = nullptr;
	UImage* HitMarkerImage = nullptr;
	UImage* KillBar = nullptr;
	
	
protected:
	virtual void NativeConstruct() override;
};
