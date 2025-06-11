// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SlotInterfaceComponent.h"
#include "Components/Image.h"
#include "Items/GameStates.h"
#include "HUD/SlotOverlay.h"

void USlotInterfaceComponent::SetVisibleSymbols(uint8 Reel, TArray<ESlotSymbols>& VisibleSymbols)
{
	if (SlotOverlayWidget == nullptr)
	{
		SlotOverlayWidget = Cast<USlotOverlay>(GetUserWidgetObject());
	}
	if (SlotOverlayWidget && AllSymbolsNotNull())
	{
		if (Reel == 0)
		{
			SlotOverlayWidget->Reel1Symbol1->SetBrushFromTexture(SymbolsToTexture2D(VisibleSymbols[0]));
			SlotOverlayWidget->Reel1Symbol2->SetBrushFromTexture(SymbolsToTexture2D(VisibleSymbols[1]));
			SlotOverlayWidget->Reel1Symbol3->SetBrushFromTexture(SymbolsToTexture2D(VisibleSymbols[2]));
		}
		else if (Reel == 1)
		{
			SlotOverlayWidget->Reel2Symbol1->SetBrushFromTexture(SymbolsToTexture2D(VisibleSymbols[0]));
			SlotOverlayWidget->Reel2Symbol2->SetBrushFromTexture(SymbolsToTexture2D(VisibleSymbols[1]));
			SlotOverlayWidget->Reel2Symbol3->SetBrushFromTexture(SymbolsToTexture2D(VisibleSymbols[2]));
		}
		else if (Reel == 2)
		{
			SlotOverlayWidget->Reel3Symbol1->SetBrushFromTexture(SymbolsToTexture2D(VisibleSymbols[0]));
			SlotOverlayWidget->Reel3Symbol2->SetBrushFromTexture(SymbolsToTexture2D(VisibleSymbols[1]));
			SlotOverlayWidget->Reel3Symbol3->SetBrushFromTexture(SymbolsToTexture2D(VisibleSymbols[2]));
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

TObjectPtr<UTexture2D> USlotInterfaceComponent::SymbolsToTexture2D(ESlotSymbols Symbol)
{
	
	switch (Symbol)
	{
		case ESlotSymbols::ESS_Cherry:		return SymbolCherryTexture;
		case ESlotSymbols::ESS_Lemon:		return SymbolLemonTexture;
		case ESlotSymbols::ESS_Watermelon:	return SymbolWatermelonTexture;
		case ESlotSymbols::ESS_Star:		return SymbolStarTexture;
		case ESlotSymbols::ESS_Bell:		return SymbolBellTexture;
		case ESlotSymbols::ESS_Diamond:		return SymbolDiamondTexture;
		case ESlotSymbols::ESS_Seven:		return SymbolSevenTexture;
		default:							return nullptr;
	}
}
