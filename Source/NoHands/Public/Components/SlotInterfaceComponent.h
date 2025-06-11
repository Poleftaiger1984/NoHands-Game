// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "SlotInterfaceComponent.generated.h"

enum class ESlotSymbols : uint8;

UCLASS()
class NOHANDS_API USlotInterfaceComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	void SetVisibleSymbols(uint8 Reel, TArray<ESlotSymbols>& VisibleSymbols);
	bool AllSymbolsNotNull();

protected:
	TObjectPtr<UTexture2D> SymbolsToTexture2D(ESlotSymbols Symbol);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UTexture2D> SymbolCherryTexture;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UTexture2D> SymbolLemonTexture;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UTexture2D> SymbolBellTexture;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UTexture2D> SymbolWatermelonTexture;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UTexture2D> SymbolDiamondTexture;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UTexture2D> SymbolStarTexture;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UTexture2D> SymbolSevenTexture;

private:
	UPROPERTY()
	TObjectPtr<class USlotOverlay> SlotOverlayWidget;
};
