// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Pickups/LuckPickup.h"
#include "Characters/CharacterStates.h"

ALuckPickup::ALuckPickup()
{
	ItemName = "LuckPickup";

	TypeOfBoost = EBoostType::EBT_Luck;
}

void ALuckPickup::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("BoostType");
}
