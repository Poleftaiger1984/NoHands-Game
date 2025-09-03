// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "ASimpleInteractable.generated.h"

class UInterctableOverlayComponent;

UCLASS()
class NOHANDS_API AASimpleInteractable : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AASimpleInteractable();
	virtual void Tick(float DeltaTime) override;
	virtual void UpdateOverlayAlignment(const FRotator& NewRotation, const FVector& NewLocation) override;
	virtual FName GetName() const override;

protected:
	virtual void EnableHighlight() override;
	virtual void DisableHighlight() override;
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditAnywhere, Category = "Item Properties")
	FName ItemName;

	// Overlay
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UInterctableOverlayComponent> InteractionOverlay;

	bool bIsOverlayActive = false;

public:
	FORCEINLINE TObjectPtr<UStaticMeshComponent> GetItemMesh() const { return ItemMesh; }

};
