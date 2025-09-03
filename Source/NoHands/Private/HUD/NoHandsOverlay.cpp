// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/NoHandsOverlay.h"
#include "Components/TextBlock.h"

void UNoHandsOverlay::SetMoney(const int32 MoneyAmount)
{
	if (MoneyText)
	{
		MoneyText->SetText(FText::FromString(FString::Printf(TEXT("%d"), MoneyAmount)));
	}
}

void UNoHandsOverlay::SetItemName(const FName& ItemName)
{
	InteractPromptText->SetText(FText::FromName(ItemName));
}

void UNoHandsOverlay::SetLuck(const int32 LuckAmount)
{
	if (LuckText)
	{
		LuckText->SetText(FText::FromString(FString::Printf(TEXT("%d"), LuckAmount)));
	}
}

void UNoHandsOverlay::EnableInteractPrompt()
{
	InteractPromptText->SetVisibility(ESlateVisibility::Visible);
}

void UNoHandsOverlay::DisableInteractPrompt()
{
	InteractPromptText->SetVisibility(ESlateVisibility::Hidden);
}
