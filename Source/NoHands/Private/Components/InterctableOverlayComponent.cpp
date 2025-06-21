// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InterctableOverlayComponent.h"
#include "Components/TextBlock.h"
#include "HUD/InteractableOverlay.h"

UInterctableOverlayComponent::UInterctableOverlayComponent()
{
	SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
}

void UInterctableOverlayComponent::SetItemName(FName& ItemName)
{
	NameOfItem = ItemName;
}

void UInterctableOverlayComponent::DisplayItemName()
{
	if (InteractableOverlay == nullptr)
	{
		InteractableOverlay = Cast<UInteractableOverlay>(GetUserWidgetObject());
	}
	if (InteractableOverlay)
	{
		InteractableOverlay->ItemName->SetText(FText::FromName(NameOfItem));
	}
}

void UInterctableOverlayComponent::SetRotation(const FRotator& NewRotation)
{
	UInterctableOverlayComponent* WidgetComp = GetOwner()->FindComponentByClass<UInterctableOverlayComponent>();

	if (WidgetComp)
	{
		WidgetComp->SetWorldRotation(NewRotation);
	}
}

void UInterctableOverlayComponent::SetLocation(const FVector& NewLocation)
{
	UInterctableOverlayComponent* WidgetComp = GetOwner()->FindComponentByClass<UInterctableOverlayComponent>();

	if (WidgetComp)
	{
		WidgetComp->SetWorldLocation(NewLocation);
	}
}
