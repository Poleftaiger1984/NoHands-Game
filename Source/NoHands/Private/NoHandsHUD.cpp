// Fill out your copyright notice in the Description page of Project Settings.


#include "NoHandsHUD.h"
#include "HUD/NoHandsOverlay.h"

void ANoHandsHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController && NoHandsOverlayClass)
		{
			NoHandsOverlay = CreateWidget<UNoHandsOverlay>(PlayerController, NoHandsOverlayClass);
			NoHandsOverlay->AddToViewport();
		}
	}
}
