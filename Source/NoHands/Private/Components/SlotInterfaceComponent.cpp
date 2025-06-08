// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SlotInterfaceComponent.h"
#include "Components/TextBlock.h"
#include "HUD/SlotOverlay.h"

void USlotInterfaceComponent::SetVisibleSymbols(TArray<FString>& VisibleSymbols)
{
	if (SlotOverlayWidget == nullptr)
	{
		SlotOverlayWidget = Cast<USlotOverlay>(GetUserWidgetObject());
	}
	if (SlotOverlayWidget && AllSymbolsNotNull())
	{
		for (int i = 0; i < VisibleSymbols.Num(); i++)
		{
			switch (i)
			{
			case 0: SlotOverlayWidget->Reel1Symbol1->SetText(FText::FromString(VisibleSymbols[i]));
			case 1: SlotOverlayWidget->Reel1Symbol2->SetText(FText::FromString(VisibleSymbols[i]));
			case 2: SlotOverlayWidget->Reel1Symbol3->SetText(FText::FromString(VisibleSymbols[i]));
			case 3: SlotOverlayWidget->Reel2Symbol1->SetText(FText::FromString(VisibleSymbols[i]));
			case 4: SlotOverlayWidget->Reel2Symbol2->SetText(FText::FromString(VisibleSymbols[i]));
			case 5: SlotOverlayWidget->Reel2Symbol3->SetText(FText::FromString(VisibleSymbols[i]));
			case 6: SlotOverlayWidget->Reel3Symbol1->SetText(FText::FromString(VisibleSymbols[i]));
			case 7: SlotOverlayWidget->Reel3Symbol2->SetText(FText::FromString(VisibleSymbols[i]));
			case 8: SlotOverlayWidget->Reel3Symbol3->SetText(FText::FromString(VisibleSymbols[i]));
			default: break;

			}
		}
	}
	
}

bool USlotInterfaceComponent::AllSymbolsNotNull()
{
	return SlotOverlayWidget->Reel1Symbol1 &&
		SlotOverlayWidget->Reel1Symbol2 &&
		SlotOverlayWidget->Reel1Symbol3 &&
		SlotOverlayWidget->Reel2Symbol1 &&
		SlotOverlayWidget->Reel2Symbol2 &&
		SlotOverlayWidget->Reel2Symbol3 &&
		SlotOverlayWidget->Reel3Symbol1 &&
		SlotOverlayWidget->Reel3Symbol2 &&
		SlotOverlayWidget->Reel3Symbol3;
}
