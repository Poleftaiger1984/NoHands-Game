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
	void UpdateOverlay(const FRotator& NewRotation, const FVector& NewLocation, bool bIsBeingLookedAt);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void HideOverlay();
	void ShowOverlay();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditAnywhere, Category = "Item Properties")
	FName ItemName;

	// Overlay
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UInterctableOverlayComponent> InteractionOverlay;

public:
	FORCEINLINE TObjectPtr<UStaticMeshComponent> GetItemMesh() const { return ItemMesh; }
	FORCEINLINE FName GetItemName() const { return ItemName; }	

};
