// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NoHandsHUD.generated.h"

class UNoHandsOverlay;

UCLASS()
class NOHANDS_API ANoHandsHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void PreInitializeComponents() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UNoHandsOverlay> NoHandsOverlayClass;

	UPROPERTY()
	TObjectPtr<UNoHandsOverlay> NoHandsOverlay;

public:
	FORCEINLINE TObjectPtr<UNoHandsOverlay> GetOverlay() const { return NoHandsOverlay; }
	
};
