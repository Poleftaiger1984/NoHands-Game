// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Pickup.h"

void APickup::InteractAction()
{
	this->Destroy();
}

void APickup::BeginPlay()
{
	Super::BeginPlay();
}
