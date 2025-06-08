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

public:
	FORCEINLINE float GetGameCost() const { return GameCost; }
};
