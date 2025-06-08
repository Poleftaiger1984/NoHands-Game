// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "SlotInterfaceComponent.generated.h"

/**
 * 
 */
UCLASS()
class NOHANDS_API USlotInterfaceComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	void SetVisibleSymbols(TArray<FString>& VisibleSymbols);
	bool AllSymbolsNotNull();

private:
	UPROPERTY()
	TObjectPtr<class USlotOverlay> SlotOverlayWidget;
};
