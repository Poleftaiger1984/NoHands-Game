// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ASimpleInteractable.h"
#include "Components/InterctableOverlayComponent.h"

AASimpleInteractable::AASimpleInteractable()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh Component"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	RootComponent = ItemMesh;

	InteractionOverlay = CreateDefaultSubobject<UInterctableOverlayComponent>(TEXT("Interaction Overlay"));
	InteractionOverlay->SetupAttachment(GetRootComponent());

	ItemMesh->SetSimulatePhysics(false);
	ItemName = "Default";

}
void AASimpleInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AASimpleInteractable::UpdateOverlay(const FRotator& NewRotation, const FVector& NewLocation, bool bIsBeingLookedAt)
{
	if (bIsBeingLookedAt && InteractionOverlay)
	{
		ShowOverlay();
		InteractionOverlay->SetRotation(NewRotation);
		InteractionOverlay->SetLocation(NewLocation);
	}
	else
	{
		HideOverlay();
	}
}

void AASimpleInteractable::BeginPlay()
{
	Super::BeginPlay();	
	Tags.Add(TEXT("Interactable"));
}

void AASimpleInteractable::HideOverlay()
{
	if (InteractionOverlay)
	{
		InteractionOverlay->SetVisibility(false);
	}
}

void AASimpleInteractable::ShowOverlay()
{
	if (InteractionOverlay)
	{
		InteractionOverlay->SetItemName(ItemName);
		InteractionOverlay->DisplayItemName();
		InteractionOverlay->SetVisibility(true);
	}
}

