// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"
#include "CharacterStates.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAttributeComponent::UseStamina(float StaminaCost, float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost * DeltaTime, 0.f, MaxStamina);
}

void UAttributeComponent::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}

void UAttributeComponent::AddBoost(EBoostType TypeOfBoost, float BoostAmount)
{
	switch (TypeOfBoost)
	{
	case EBoostType::EBT_Health:
	{
		Health += BoostAmount;
		break;
	}
	case EBoostType::EBT_Stamina:
	{
		Stamina += BoostAmount;
		break;
	}
	case EBoostType::EBT_Money:
	{
		Money += BoostAmount;
		break;
	}
	default:
		break;
	}
}

void UAttributeComponent::LoseAttribute(EBoostType TypeOfBoost, float AmountToLose)
{
	switch (TypeOfBoost)
	{
	case EBoostType::EBT_Health:
	{
		Health -= AmountToLose;
		break;
	}
	case EBoostType::EBT_Stamina:
	{
		Stamina -= AmountToLose;
		break;
	}
	case EBoostType::EBT_Money:
	{
		Money -= AmountToLose;
		break;
	}
	default:
		break;
	}
}
