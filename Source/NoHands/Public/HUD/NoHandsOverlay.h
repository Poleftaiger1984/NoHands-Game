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
	void SetMoney(int32  MoneyAmount);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MoneyText;
};
