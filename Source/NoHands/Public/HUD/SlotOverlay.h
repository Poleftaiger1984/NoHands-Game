// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlotOverlay.generated.h"

class UImage;

UCLASS()
class NOHANDS_API USlotOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Reel1Symbol1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Reel1Symbol2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Reel1Symbol3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Reel2Symbol1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Reel2Symbol2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Reel2Symbol3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Reel3Symbol1;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Reel3Symbol2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Reel3Symbol3;

};
