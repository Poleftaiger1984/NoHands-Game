// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InterctableOverlayComponent.generated.h"

class UInteractableOverlay;

UCLASS()
class NOHANDS_API UInterctableOverlayComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UInterctableOverlayComponent();
	void SetItemName(FName& ItemName);
	void DisplayItemName();
	void SetRotation(const FRotator& NewRotation);
	void SetLocation(const FVector& NewLocation);

protected:
	FName NameOfItem;

private:
	UPROPERTY()
	TObjectPtr<UInteractableOverlay> InteractableOverlay;
};
