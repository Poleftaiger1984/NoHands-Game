// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ASimpleInteractable.h"
#include "Components/InterctableOverlayComponent.h"
#include <NoHands/NoHands.h>

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
	ItemMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	ItemName = "Default";

}
void AASimpleInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AASimpleInteractable::UpdateOverlayAlignment(const FRotator& NewRotation, const FVector& NewLocation)
{
	if (bIsOverlayActive && InteractionOverlay)
	{
		InteractionOverlay->SetRotation(NewRotation);
		InteractionOverlay->SetLocation(NewLocation);
	}
	
}

FName AASimpleInteractable::GetName() const
{
	return ItemName;
}

void AASimpleInteractable::EnableHighlight()
{
	if (InteractionOverlay)
	{
		bIsOverlayActive = true;
		InteractionOverlay->SetItemName(ItemName);
		InteractionOverlay->DisplayItemName();
		InteractionOverlay->SetVisibility(true);
	}
	ItemMesh->SetRenderCustomDepth(true);
}

void AASimpleInteractable::DisableHighlight()
{
	if (InteractionOverlay)
	{
		bIsOverlayActive = false;
		InteractionOverlay->SetVisibility(false);
	}
	ItemMesh->SetRenderCustomDepth(false);
}

void AASimpleInteractable::BeginPlay()
{
	Super::BeginPlay();	
	Tags.Add(TEXT("Interactable"));
}

