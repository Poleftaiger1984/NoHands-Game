// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ASimpleInteractable.h"

AASimpleInteractable::AASimpleInteractable()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh Component"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	RootComponent = ItemMesh;

	ItemMesh->SetSimulatePhysics(false);
	ItemName = "Default";

}
void AASimpleInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AASimpleInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}



