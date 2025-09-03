// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Pickup.h"
#include "LuckPickup.generated.h"

/**
 * 
 */
UCLASS()
class NOHANDS_API ALuckPickup : public APickup
{
	GENERATED_BODY()

public:
	ALuckPickup();

protected:
	virtual void BeginPlay() override;	
};
