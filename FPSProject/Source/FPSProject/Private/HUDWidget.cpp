// Copyright © 2018, AJACKS

#include "HUDWidget.h"
#include "WidgetTree.h"


void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	DeadTextBlock = Cast<UTextBlock>(WidgetTree->FindWidget(FName("DeadText")));
	HitMarkerImage = Cast<UImage>(WidgetTree->FindWidget(FName("HitMarker")));
	KillBar = Cast<UImage>(WidgetTree->FindWidget(FName("KillBar")));
}

