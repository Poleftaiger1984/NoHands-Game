// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Money.h"
#include "CharacterStates.h"

AMoney::AMoney()
{
	ItemName = "Money";
	
	TypeOfBoost = EBoostType::EBT_Money;
}

void AMoney::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("BoostType");
}
