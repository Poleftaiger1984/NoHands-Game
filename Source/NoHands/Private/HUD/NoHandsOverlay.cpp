// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/NoHandsOverlay.h"
#include "Components/TextBlock.h"

void UNoHandsOverlay::SetMoney(int32 MoneyAmount)
{
	if (MoneyText)
	{
		MoneyText->SetText(FText::FromString(FString::Printf(TEXT("%d"), MoneyAmount)));
	}
}
