// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterStates.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAttributeComponent;
class AASimpleInteractable;
class UNoHandsOverlay;

enum class EGameState : uint8;

UCLASS()
class NOHANDS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

	virtual void Tick(float DeltaTime) override;

	void InteractionOverlayAlignment();

	void StaminaHandler(float DeltaTime);

protected:
	virtual void BeginPlay() override;

	/* Callbacks for input */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Crouch();	
	void Sprint();
	void FinishedSprint();
	void OnInteractHoldStarted();
	void OnInteractHoldTriggered();
	void OnInteractHoldCompleted();
	void OnInteractHoldCanceled();
	void Interaction();

	void InteractionTrace(FHitResult& SphereHitResult);
	void SetItemLookedAt(FHitResult& LookTraceResult);

	bool CanSprint();
	/* Callbacks for input */


	/* Input Actions */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> FPContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InteractionAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InteractionHeldAction;
	/* Input Actions */

	/* Movement Variables */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> Attributes;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float CrouchHeight = 50.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float CrouchWalkSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed = 1200.f;

	UPROPERTY(VisibleInstanceOnly, Category = "Movement")
	uint8 JumpCounter = 0;

	UPROPERTY(VisibleInstanceOnly)
	bool bIsMidair = false;
	/* Movement Variables */

	/* Interaction Variables */
	FHitResult InteractionHit;
	TObjectPtr<AActor> LastSeenActor;
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AASimpleInteractable> ItemInView;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractRange = 100.f;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float CrosshairRadius = 50.f;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	bool bShowInteractionDebug = false;
	/* Interaction Variables */

	/* Game Variables */
	EGameState GameState;

	UPROPERTY(EditAnywhere, Category = "Games")
	int32 GameBet = 0;
	/* Game Variables */

	/* HUD Variables */
	UPROPERTY(EditAnywhere, Category = "UI")
	float InteractUIOffset = 10.f;
	/* HUD Variables */

private:
	void InitHUDOverlay();

	/* Character Components */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Montages", meta = (AllowPrivateAccess = "true"))
	ECharacterState CharacterState;
	/* Character Components */

	/* HUD */
	FRotator CameraRotation;
	FVector CameraLocation;

	FTimerHandle InteractTimerHandle;
	TObjectPtr<UNoHandsOverlay> HUDOverlay;
	/* HUD */

};
