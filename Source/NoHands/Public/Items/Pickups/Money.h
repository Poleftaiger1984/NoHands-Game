// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Pickup.h"
#include "Money.generated.h"

enum class EBoostType : uint8;

UCLASS()
class NOHANDS_API AMoney : public APickup
{
	GENERATED_BODY()
	
public:
	AMoney();

protected:
	virtual void BeginPlay() override;


};
