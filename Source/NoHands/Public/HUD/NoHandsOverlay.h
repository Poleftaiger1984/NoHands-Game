// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NoHandsOverlay.generated.h"

class UTextBlock;

UCLASS()
class NOHANDS_API UNoHandsOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	void SetMoney(const int32  MoneyAmount);
	void SetItemName(const FName& ItemName);
	void SetLuck(const int32 LuckAmount);
	void EnableInteractPrompt();
	void DisableInteractPrompt();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MoneyText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LuckText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InteractPromptText;

	bool bIsOverlayActive = false;
};
