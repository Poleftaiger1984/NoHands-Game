// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/InteractionInterface.h"
#include "Items/GameStates.h"

// Add default functionality here for any IInteractionInterface functions that are not pure virtual.

void IInteractionInterface::InteractAction()
{
}

void IInteractionInterface::SetBet(int32 PlayerBet)
{

}

int32 IInteractionInterface::GetWinnings() const
{
	return int32();
}

EGameState IInteractionInterface::GetGameState() const
{
	return EGameState();
}

