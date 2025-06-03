// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ASimpleInteractable.h"
#include "Playable.generated.h"

enum class EGameState : uint8;

UCLASS()
class NOHANDS_API APlayable : public AASimpleInteractable
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Game Settings | Defaults")
	float GameCost = 5;

protected:

	UPROPERTY(EditInstanceOnly, Category = "Game Settings | Defaults")
	EGameState GameState;

	//Player input

	uint8 Bet = 0;

	//Payouts

	uint8 TotalPayout;

public:
	FORCEINLINE float GetGameCost() const { return GameCost; }
	FORCEINLINE void SetBet(uint8 PlayerBet) { Bet = PlayerBet; GameState = EGameState::EGS_Playing; }
	FORCEINLINE uint8 GetWinnings() const { return TotalPayout; }
	FORCEINLINE EGameState GetGameState() const { return GameState; }
};
