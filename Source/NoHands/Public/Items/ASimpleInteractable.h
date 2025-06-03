// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "ASimpleInteractable.generated.h"

UCLASS()
class NOHANDS_API AASimpleInteractable : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AASimpleInteractable();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditAnywhere, Category = "Item Properties")
	FName ItemName;

public:
	FORCEINLINE TObjectPtr<UStaticMeshComponent> GetItemMesh() const { return ItemMesh; }
	FORCEINLINE FName GetItemName() const { return ItemName; }

};
