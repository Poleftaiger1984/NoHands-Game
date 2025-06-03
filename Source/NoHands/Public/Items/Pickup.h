// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ASimpleInteractable.h"
#include "Pickup.generated.h"

enum class EBoostType : uint8;

UCLASS()
class NOHANDS_API APickup : public AASimpleInteractable
{
	GENERATED_BODY()

public:
	virtual void InteractAction() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Item Properties")
	float AmountOfBoost = 0;

	EBoostType TypeOfBoost;

public:
	FORCEINLINE float GetAmount() const { return AmountOfBoost; }
	FORCEINLINE EBoostType GetTypeOfBoost() const { return TypeOfBoost; }
};
